// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetBlackBoardBool.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_SetBlackBoardBool::UBTT_SetBlackBoardBool(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "SetBoolForPath";
}

EBTNodeResult::Type UBTT_SetBlackBoardBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const BC = OwnerComp.GetBlackboardComponent();
	if (!BC)
	{
		return EBTNodeResult::Failed;
	}

	BC->SetValueAsBool(BlackboardKey.SelectedKeyName,bSetToTrue);
	BC->SetValueAsBool(BlackboardKey.SelectedKeyName,bSetToFalse);
	
	return EBTNodeResult::Succeeded;
}
