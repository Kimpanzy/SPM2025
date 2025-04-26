// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RatAIController.generated.h"

/**
 * 
 */
UCLASS()
class SPM2025_API ARatAIController : public AAIController
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> Targets;

	UPROPERTY(BlueprintReadWrite)
	int CurrentIndex;

	TArray<FVector> GetAllTargets();

	int GetCurrentIndex();
	void SetCurrentIndex(int NewIndex);
};
