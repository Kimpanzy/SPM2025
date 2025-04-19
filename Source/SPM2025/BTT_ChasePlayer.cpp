// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"

#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& ObjectInitializer):
	UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get target location från blackboard via npc controller
	if (auto* const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		//Gå till player location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);
		
		//Avsluta med success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
