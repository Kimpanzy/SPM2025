// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightraySource.generated.h"

UCLASS()
class SPM2025_API ALightraySource : public AActor
{
	GENERATED_BODY()

public:
	ALightraySource();

private:
	UPROPERTY(EditAnywhere, BlueprintGetter=GetMaxBounces)
	int MaxBounces = 20;
	int Bounces = 0;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetRayLength)
	float RayLength = 500.f;

	TWeakObjectPtr<AActor> LastHitTarget;

	FTimerHandle TickTimerHandle;

	void CastLightrayFrom(FVector Source, FVector Direction, const AActor* PreviousHit);

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	int GetMaxBounces() const { return MaxBounces; }

	UFUNCTION(BlueprintPure)
	float GetRayLength() const { return RayLength; }

	UFUNCTION(BlueprintCallable)
	void CastLightray();
};
