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

	UPROPERTY(BlueprintReadWrite)
	AActor* Cheese;

	UPROPERTY(BlueprintReadWrite)
	bool bPlayerIsHoldingCheese;
	
	TArray<FVector> GetAllTargets();

	int GetCurrentIndex() const;
	void SetCurrentIndex(int NewIndex);
	AActor* GetCheese() const;
};
