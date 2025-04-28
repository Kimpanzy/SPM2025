// Fill out your copyright notice in the Description page of Project Settings.


#include "RatAIController.h"

#include "Kismet/GameplayStatics.h"

void ARatAIController::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
}
TArray<FVector> ARatAIController::GetAllTargets()
{
	return  Targets;
}

int ARatAIController::GetCurrentIndex() const
{
	return CurrentIndex;
}

void ARatAIController::SetCurrentIndex(int NewIndex)
{
	CurrentIndex = NewIndex;
}

AActor* ARatAIController::GetCheese() const
{
	return Cheese;
}
