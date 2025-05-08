// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LookAt.h"

#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTT_LookAt::UBTT_LookAt(FObjectInitializer const& ObjectInitializer):
UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = "Look At";
}

EBTNodeResult::Type UBTT_LookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const BC = OwnerComp.GetBlackboardComponent();
	auto* const Cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());

	if (!BC || !Cont)
	{
		return EBTNodeResult::Failed;
	}
	APawn* NPC = Cont->GetPawn();
	auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
	if (NPC && Player && BC->GetValueAsBool("IsHiding"))
	{
		FVector Direction = Player->GetActorLocation() - NPC->GetActorLocation();
		Direction.Z = 0;
		FRotator LookAtDirection = FRotationMatrix::MakeFromX(Direction).Rotator();

		NPC->SetActorRotation(LookAtDirection);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
