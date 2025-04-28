// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LookAtCheese.h"

#include "RatAIController.h"

UBTT_LookAtCheese::UBTT_LookAtCheese()
{
	NodeName = "Look At Cheese";
}

EBTNodeResult::Type UBTT_LookAtCheese::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARatAIController* RatAICon = Cast<ARatAIController>(OwnerComp.GetAIOwner());
	if (!RatAICon)
	{
		return EBTNodeResult::Failed;
	}
	FVector DirToCheese = RatAICon->Cheese->GetActorLocation() - RatAICon->GetPawn()->GetActorLocation();
	
	RatAICon->GetPawn()->SetActorRotation(FRotator(0.f,DirToCheese.Rotation().Yaw,0.f));
	return EBTNodeResult::Succeeded;
}
