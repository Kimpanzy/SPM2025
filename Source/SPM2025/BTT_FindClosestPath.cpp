// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindClosestPath.h"

#include "NPC_AIController.h"

UBTT_FindClosestPath::UBTT_FindClosestPath()
{
	NodeName = "FindClosestPath";
}

EBTNodeResult::Type UBTT_FindClosestPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const BC = OwnerComp.GetBlackboardComponent())
		{
			
		}
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
