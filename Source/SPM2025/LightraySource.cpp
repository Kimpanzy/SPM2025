// Fill out your copyright notice in the Description page of Project Settings.


#include "LightraySource.h"

#include "LightrayTarget.h"
#include "Mirror.h"

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

	AActor* HitMirror = nullptr;
	for (int i = 0; i < MaxBounces; ++i)
	{
		const FVector End = Source + Direction * RayLength;

		QueryParams.ClearIgnoredSourceObjects();
		if (HitMirror)
		{
			QueryParams.AddIgnoredSourceObject(HitMirror);
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

			AActor* HitActor = HitResult.GetActor();
			if (HitResult.GetActor()->IsA(AMirror::StaticClass()))
			{
				HitMirror = HitActor;
				Source = HitResult.ImpactPoint;
				Direction = HitResult.ImpactNormal;
			}
			else
			{
				if (
					const bool PreviousTargetValid = LastHitTarget.IsValid() && LastHitTarget->IsValidLowLevel();
					!PreviousTargetValid || LastHitTarget.Get() != HitActor
				)
				{
					if (PreviousTargetValid)
					{
						ILightrayTarget::Execute_OnRayStopHitting(LastHitTarget.Get());
						LastHitTarget.Reset();
					}

					// Cast does not work for Blueprint implementations!
					if (HitActor->Implements<ULightrayTarget>())
					{
						LastHitTarget = HitActor;
						ILightrayTarget::Execute_OnRayHit(HitActor);
					}
				}

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
}
