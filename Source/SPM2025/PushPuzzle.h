// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PushPuzzle.generated.h"

UCLASS()
class SPM2025_API APushPuzzle : public AActor
{
	GENERATED_BODY()

public:
	APushPuzzle();

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TArray<class APushPuzzleGoal*> Goals;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	class AMovableWall* MovableWall = nullptr;

	int Count = 0;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnGoalComplete(APushPuzzleGoal* Goal);
};
