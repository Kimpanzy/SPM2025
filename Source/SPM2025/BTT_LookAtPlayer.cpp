// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LookAtPlayer.h"

#include "RatAIController.h"

UBTT_LookAtPlayer::UBTT_LookAtPlayer()
{
	NodeName = "Look At Player";
}

EBTNodeResult::Type UBTT_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARatAIController* RatAICon = Cast<ARatAIController>(OwnerComp.GetAIOwner());
	if (!RatAICon)
	{
		return EBTNodeResult::Failed;
	}
	FVector DirToPlayer = RatAICon->Player->GetActorLocation() - RatAICon->GetPawn()->GetActorLocation();
	
	RatAICon->GetPawn()->SetActorRotation(FRotator(0.f,DirToPlayer.Rotation().Yaw,0.f));
	return EBTNodeResult::Succeeded;

}
