// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PushPuzzleGoal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushPuzzleCompleteDelegate, class APushPuzzleGoal*, Goal);

UCLASS()
class SPM2025_API APushPuzzleGoal : public AActor
{
	GENERATED_BODY()

public:
	APushPuzzleGoal();

	UPROPERTY(BlueprintAssignable)
	FPushPuzzleCompleteDelegate OnCompleted;

	void Complete();
};
