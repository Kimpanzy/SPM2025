// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetBoolForPath.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_SetBoolForPath::UBTT_SetBoolForPath(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "SetBoolForPath";
}

EBTNodeResult::Type UBTT_SetBoolForPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const BC = OwnerComp.GetBlackboardComponent();
	if (!BC)
	{
		return EBTNodeResult::Failed;
	}

	bool bCurrentValue = BC->GetValueAsBool(BoolKey.SelectedKeyName);

	BC->SetValueAsBool(BoolKey.SelectedKeyName, !bCurrentValue);
	
	return EBTNodeResult::Succeeded;
}
