// Fill out your copyright notice in the Description page of Project Settings.


#include "PushPuzzle.h"

#include "MovableWall.h"
#include "PushPuzzleGoal.h"

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
}

void APushPuzzle::OnGoalComplete(APushPuzzleGoal* Goal)
{
	if (Goals.Num() <= ++Count && MovableWall)
	{
		MovableWall->Move();
	}
}
