// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBlock.h"

APushableBlock::APushableBlock()
{
 	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APushableBlock::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APushableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

