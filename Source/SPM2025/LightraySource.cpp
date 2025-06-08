// Fill out your copyright notice in the Description page of Project Settings.


#include "LightraySource.h"

#include "LightrayTarget.h"
#include "Mirror.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#define TICK_RATE .2f

ALightraySource::ALightraySource()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALightraySource::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TickTimerHandle, this, &ALightraySource::CastLightray, TICK_RATE, true);
}

void ALightraySource::CastLightray()
{
	CastLightrayFrom(GetActorLocation(), GetActorForwardVector());
}

void ALightraySource::CastLightrayFrom(FVector Source, FVector Direction)
{
	const FCollisionObjectQueryParams CollisionObjectParams(
		ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)
	);

	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;

	TInlineComponentArray<UNiagaraComponent*> NiagaraSystems;
	GetComponents(UNiagaraComponent::StaticClass(), NiagaraSystems);

	AActor* PreviousHit = nullptr;
	int i = 0;
	for (; i < MaxBounces; ++i)
	{
		const FVector End = Source + Direction * RayLength;

		QueryParams.ClearIgnoredSourceObjects();
		if (PreviousHit)
		{
			QueryParams.AddIgnoredSourceObject(PreviousHit);
		}

		if (
			FHitResult HitResult;
			GetWorld()->LineTraceSingleByObjectType(
				HitResult,
				Source,
				End,
				CollisionObjectParams,
				QueryParams
			)
		)
		{
#if WITH_EDITOR
			if (GetWorld()->IsPlayInEditor())
			{
#endif
				UNiagaraComponent* Lightray;

				if (
					i < NiagaraSystems.Num())
				{
					Lightray = NiagaraSystems[i];
				}
				else
				{
					Lightray = UNiagaraFunctionLibrary::SpawnSystemAttached(
						LightrayNS,
						GetRootComponent(),
						NAME_None,
						FVector::ZeroVector,
						FRotator::ZeroRotator,
						EAttachLocation::Type::KeepRelativeOffset,
						false,
						false
					);

					Lightray->SetFloatParameter(TEXT("Lifetime"), TICK_RATE);
				}

				Lightray->SetVectorParameter(TEXT("Beam Start"), Source);
				Lightray->SetVectorParameter(TEXT("Beam End"), HitResult.ImpactPoint);
				Lightray->Activate(false);
#if WITH_EDITOR
			}
			else
			{
				DrawDebugLine(GetWorld(), Source, HitResult.ImpactPoint, {255, 0, 0}, false, TICK_RATE);
				DrawDebugLine(GetWorld(), HitResult.ImpactPoint, End, {0, 255, 0}, false, TICK_RATE);
			}
#endif

			if (
				const AActor* HitActor = PreviousHit = HitResult.GetActor();
				const AMirror* Mirror = Cast<AMirror>(HitActor)
			)
			{
				if (Mirror->MirrorMeshComponent)
				{
					if (
						const FVector MirrorForwardVector = UKismetMathLibrary::GetForwardVector(
							Mirror->MirrorMeshComponent->GetComponentRotation());
						Direction.Dot(MirrorForwardVector) < .40)
					{
						Source = HitResult.ImpactPoint;
						Direction = HitResult.ImpactNormal;
					}
					else
					{
						// Not bouncing on mirror front surface, exit early
						break;
					}
				}
				else
				{
					// Mirror missing mesh, exit early
					break;
				}
			}
			else
			{
				// Exit early if not bouncing on mirror
				break;
			}
		}
		else
		{
#if WITH_EDITOR
			if (GetWorld()->IsPlayInEditor())
			{
#endif
				UNiagaraComponent* Lightray;

				if (
					i < NiagaraSystems.Num())
				{
					Lightray = NiagaraSystems[i];
				}
				else
				{
					Lightray = UNiagaraFunctionLibrary::SpawnSystemAttached(
						LightrayNS,
						GetRootComponent(),
						NAME_None,
						FVector::ZeroVector,
						FRotator::ZeroRotator,
						EAttachLocation::Type::KeepRelativeOffset,
						false,
						false
					);

					Lightray->SetFloatParameter(TEXT("Lifetime"), TICK_RATE);
				}

				Lightray->SetVectorParameter(TEXT("Beam Start"), Source);
				Lightray->SetVectorParameter(TEXT("Beam End"), End);
				Lightray->Activate(false);
#if WITH_EDITOR
			}
			else
			{
				DrawDebugLine(GetWorld(), Source, End, {255, 0, 0}, false, TICK_RATE);
			}
#endif

			// No hit, exit loop
			break;
		}
	}

	// Account for last loop iteration
	++i;
	for (; i < NiagaraSystems.Num(); ++i)
	{
		NiagaraSystems[i]->DeactivateImmediate();
	}

	if (
		const bool PreviousTargetValid = LastHitTarget.IsValid() && LastHitTarget->IsValidLowLevel();
		!PreviousTargetValid || LastHitTarget.Get() != PreviousHit
	)
	{
		if (PreviousTargetValid)
		{
			ILightrayTarget::Execute_OnRayStopHitting(LastHitTarget.Get());
			LastHitTarget.Reset();
		}

		// Cast does not work for Blueprint implementations!
		if (PreviousHit && PreviousHit->Implements<ULightrayTarget>())
		{
			LastHitTarget = PreviousHit;
			ILightrayTarget::Execute_OnRayHit(PreviousHit);
		}
	}
}
