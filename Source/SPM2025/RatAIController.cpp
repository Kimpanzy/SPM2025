// Fill out your copyright notice in the Description page of Project Settings.


#include "RatAIController.h"

TArray<FVector> ARatAIController::GetAllTargets()
{
	return  Targets;
}

int ARatAIController::GetCurrentIndex()
{
	return CurrentIndex;
}

void ARatAIController::SetCurrentIndex(int NewIndex)
{
	CurrentIndex = NewIndex;
}
