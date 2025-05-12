// Fill out your copyright notice in the Description page of Project Settings.


#include "LightraySource.h"

#include "LightrayTarget.h"
#include "Mirror.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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

	AActor* PreviousHit = nullptr;
	for (int i = 0; i < MaxBounces; ++i)
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
			DrawDebugLine(GetWorld(), Source, HitResult.ImpactPoint, {255, 0, 0}, false, TICK_RATE);
			DrawDebugLine(GetWorld(), HitResult.ImpactPoint, End, {0, 255, 0}, false, TICK_RATE);
#endif

			UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAttached(
				LightrayNS,
				GetRootComponent(),
				NAME_None,
				Source,
				FRotator::ZeroRotator,
				EAttachLocation::Type::KeepWorldPosition,
				false
			);


			//NC->SetVectorParameter(TEXT("Beam End"), GetTransform().InverseTransformPosition(HitResult.ImpactPoint));

			if (
				const AActor* HitActor = PreviousHit = HitResult.GetActor();
				HitActor->IsA(AMirror::StaticClass())
			)
			{
				Source = HitResult.ImpactPoint;
				Direction = HitResult.ImpactNormal;
			}
			else
			{
				// Exit early if not bouncing on mirror
				break;
			}
		}
#if WITH_EDITOR
		else
		{
			DrawDebugLine(GetWorld(), Source, End, {255, 0, 0}, false, TICK_RATE);
		}
#endif
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
