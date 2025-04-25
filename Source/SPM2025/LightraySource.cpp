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
	CastLightrayFrom(GetActorLocation(), GetActorForwardVector(), nullptr);
	// TODO FIXME Make this not required to stop flickering, by removing recursion and use a while loop in CastLightrayFrom
	Bounces = 0;
}

void ALightraySource::CastLightrayFrom(const FVector Source, const FVector Direction, const AActor* PreviousHit)
{
	const FVector End = Source + Direction * RayLength;

	const FCollisionObjectQueryParams CollisionObjectParams(
		ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic));

	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;

	if (PreviousHit)
	{
		QueryParams.AddIgnoredActor(PreviousHit);
	}

	if (FHitResult HitResult; GetWorld()->LineTraceSingleByObjectType(HitResult, Source, End, CollisionObjectParams,
	                                                                  QueryParams))
	{
#if WITH_EDITOR
		DrawDebugLine(GetWorld(), Source, HitResult.ImpactPoint, {255, 0, 0}, false, TICK_RATE);
		DrawDebugLine(GetWorld(), HitResult.ImpactPoint, End, {0, 255, 0}, false, TICK_RATE);
#endif

		AActor* HitActor = HitResult.GetActor();

		if (HitActor->IsA(AMirror::StaticClass()))
		{
			if (++Bounces <= MaxBounces)
			{
				CastLightrayFrom(HitResult.ImpactPoint, HitResult.ImpactNormal, HitActor);
			}
		}

		if (LastHitTarget != HitActor)
		{
			// Cast does not work for Blueprint implementations!
			if (HitActor->Implements<ULightrayTarget>())
			{
				LastHitTarget = HitActor;
				// Cast does not work for Blueprint implementations!
				TScriptInterface<ILightrayTarget>(HitActor)->Execute_OnRayHit(HitActor);
			}
			else if (LastHitTarget.IsValid() && LastHitTarget->IsValidLowLevel())
			{
				TScriptInterface<ILightrayTarget>(LastHitTarget.Get())->Execute_OnRayStopHitting(LastHitTarget.Get());
				LastHitTarget = nullptr;
			}
		}
	}
#if WITH_EDITOR
	else
	{
		DrawDebugLine(GetWorld(), Source, End, {255, 0, 0}, false, TICK_RATE);
	}
#endif
}
