// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PushableBallGoal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushBallPuzzleCompleteDelegate, class APushableBallGoal*, Goal);

UCLASS()
class SPM2025_API APushableBallGoal : public AActor
{
	GENERATED_BODY()

public:
	APushableBallGoal();

protected:
	virtual void BeginPlay() override;

public:
	bool bIsComplete = false;

	UPROPERTY(BlueprintReadWrite)
	UBoxComponent* TriggerBox = nullptr;

	UPROPERTY(BlueprintAssignable)
	FPushBallPuzzleCompleteDelegate OnCompleted;

	void Complete();

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
	               AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	               const FHitResult& SweepResult);
};
