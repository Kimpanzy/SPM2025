// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_LookAtCheese.generated.h"

/**
 * 
 */
UCLASS()
class SPM2025_API UBTT_LookAtCheese : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_LookAtCheese();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
