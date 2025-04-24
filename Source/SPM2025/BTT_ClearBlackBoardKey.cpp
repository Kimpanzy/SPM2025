// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ClearBlackBoardKey.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_ClearBlackBoardKey::UBTT_ClearBlackBoardKey(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Clear Blackboard Key");
}

EBTNodeResult::Type UBTT_ClearBlackBoardKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const BC = OwnerComp.GetBlackboardComponent();
	if (!BC)
	{
		return EBTNodeResult::Failed;
	}

	BC->ClearValue(BlackboardKey.SelectedKeyName);
	
	return EBTNodeResult::Succeeded;
}
