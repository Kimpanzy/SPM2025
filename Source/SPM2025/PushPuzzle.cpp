// Fill out your copyright notice in the Description page of Project Settings.


#include "PushPuzzle.h"

#include "MovableWall.h"
#include "PushPuzzleGoal.h"
#include "RequiemSaveGame.h"

APushPuzzle::APushPuzzle()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APushPuzzle::BeginPlay()
{
	Super::BeginPlay();

	for (APushPuzzleGoal* Goal : Goals)
	{
		if (Goal)
		{
			Goal->OnCompleted.AddDynamic(this, &APushPuzzle::OnGoalComplete);
		}
	}

	URequiemGameInstance::Execute_RequestLoad(URequiemGameInstance::GetInstance(GetWorld()), this);
}

void APushPuzzle::OnGoalComplete(APushPuzzleGoal* Goal)
{
	if (Goals.Num() <= ++CompletedGoals && MovableWall)
	{
		MovableWall->Move();
	}
}

void APushPuzzle::SaveData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	FPushPuzzleSaveData SaveData;
	SaveData.Goals.Reserve(Goals.Num());

	for (int i = 0; i < Goals.Num(); ++i)
	{
		SaveData.Goals.Add(Goals[i]->IsHidden());
	}

	SaveGameInstance->PushPuzzles.Add(ID, SaveData);
}

void APushPuzzle::LoadData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	if (FPushPuzzleSaveData* SaveData = SaveGameInstance->PushPuzzles.Find(ID))
	{
		for (int i = 0; i < Goals.Num(); ++i)
		{
			if (i < SaveData->Goals.Num() && SaveData->Goals[i])
			{
				Goals[i]->SetActorHiddenInGame(true);
				if (Goals.Num() <= ++CompletedGoals && MovableWall)
				{
					MovableWall->Move();
				}
			}
		}
	}
}
