// Fill out your copyright notice in the Description page of Project Settings.
#include "BallPushPuzzle.h"

#include "MovableWall.h"
#include "PushableBallGoal.h"
#include "PushBall.h"
#include "RequiemSaveGame.h"
#include "Kismet/GameplayStatics.h"

ABallPushPuzzle::ABallPushPuzzle()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABallPushPuzzle::BeginPlay()
{
	Super::BeginPlay();

	URequiemGameInstance::Execute_RequestLoad(URequiemGameInstance::GetInstance(GetWorld()), this);

	for (APushableBallGoal* Goal : Goals)
	{
		if (Goal)
		{
			Goal->OnCompleted.AddDynamic(this, &ABallPushPuzzle::OnGoalComplete);
		}
	}
}

void ABallPushPuzzle::OnGoalComplete(APushableBallGoal* Goal)
{
	if (Goals.Num() == ++CompletedGoals && MovableWall)
	{
		MovableWall->Move();
		URequiemGameInstance::Execute_RequestSave(URequiemGameInstance::GetInstance(GetWorld()), true);
	}
}

void ABallPushPuzzle::SaveData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	FBallPushPuzzleSaveData SaveData;
	SaveData.Goals.Reserve(Goals.Num());

	for (int i = 0; i < Goals.Num(); ++i)
	{
		APushBall* BallInGoal = Goals[i]->Ball;
		SaveData.Goals.Add(BallInGoal ? BallInGoal->ID : FGuid());
	}

	SaveGameInstance->PushableBallPuzzles.Add(ID, SaveData);
}

void ABallPushPuzzle::LoadData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	if (FBallPushPuzzleSaveData* SaveData = SaveGameInstance->PushableBallPuzzles.Find(ID))
	{
		TArray<AActor*> BallActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APushBall::StaticClass(), BallActors);

		for (int i = 0; i < Goals.Num(); ++i)
		{
			if (i < SaveData->Goals.Num() && SaveData->Goals[i].IsValid())
			{
				for (int j = 0; j < BallActors.Num(); ++j)
				{
					if (
						APushBall* BallInGoal = static_cast<APushBall*>(BallActors[j]);
						BallInGoal->ID == SaveData->Goals[i]
					)
					{
						Goals[i]->Complete(BallInGoal);
						if (Goals.Num() == ++CompletedGoals && MovableWall)
						{
							MovableWall->Move();
						}

						break;
					}
				}
			}
		}
	}
}
