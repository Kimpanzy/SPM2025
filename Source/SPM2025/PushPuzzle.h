// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saveable.h"
#include "GameFramework/Actor.h"
#include "PushPuzzle.generated.h"

UCLASS()
class SPM2025_API APushPuzzle : public AActor, public ISaveable
{
	GENERATED_BODY()

public:
	APushPuzzle();

private:
	UPROPERTY(VisibleAnywhere, NonPIEDuplicateTransient)
	FGuid ID = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TArray<class APushPuzzleGoal*> Goals;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	class AMovableWall* MovableWall = nullptr;

	int CompletedGoals = 0;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnGoalComplete(APushPuzzleGoal* Goal);

private:
	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void SaveData_Implementation(URequiemSaveGame* SaveGameInstance) override;

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void LoadData_Implementation(URequiemSaveGame* SaveGameInstance) override;
};

USTRUCT()
struct SPM2025_API FPushPuzzleSaveData
{
	GENERATED_BODY()

	TBitArray<FDefaultBitArrayAllocator> Goals;
};
