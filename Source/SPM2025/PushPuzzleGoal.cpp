// Fill out your copyright notice in the Description page of Project Settings.


#include "PushPuzzleGoal.h"

APushPuzzleGoal::APushPuzzleGoal()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APushPuzzleGoal::Complete()
{
	OnCompleted.Broadcast(this);
	SetActorHiddenInGame(true);
}
