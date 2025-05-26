// Fill out your copyright notice in the Description page of Project Settings.
#include "BallPushPuzzle.h"

#include "MovableWall.h"
#include "PushableBallGoal.h"
#include "RequiemSaveGame.h"

ABallPushPuzzle::ABallPushPuzzle()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABallPushPuzzle::BeginPlay()
{
	Super::BeginPlay();

	for (APushableBallGoal* Goal : Goals)
	{
		if (Goal)
		{
			Goal->OnCompleted.AddDynamic(this, &ABallPushPuzzle::OnGoalComplete);
		}
	}

	URequiemGameInstance::Execute_RequestLoad(URequiemGameInstance::GetInstance(GetWorld()), this);
}

void ABallPushPuzzle::OnGoalComplete(APushableBallGoal* Goal)
{
	if (Goals.Num() == ++CompletedGoals && MovableWall)
	{
		MovableWall->Move();
	}
}

void ABallPushPuzzle::SaveData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	FBallPushPuzzleSaveData SaveData;
	SaveData.Goals.Reserve(Goals.Num());

	for (int i = 0; i < Goals.Num(); ++i)
	{
		SaveData.Goals.Add(Goals[i]->bIsComplete);
	}

	SaveGameInstance->PushableBalls.Add(ID, SaveData);
}

void ABallPushPuzzle::LoadData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	if (FBallPushPuzzleSaveData* SaveData = SaveGameInstance->PushableBalls.Find(ID))
	{
		for (int i = 0; i < Goals.Num(); ++i)
		{
			if (i < SaveData->Goals.Num() && SaveData->Goals[i])
			{
				Goals[i]->Complete();
				if (Goals.Num() == CompletedGoals)
				{
					URequiemGameInstance::Execute_RequestSave(URequiemGameInstance::GetInstance(GetWorld()), true);
				}
			}
		}
	}
}
