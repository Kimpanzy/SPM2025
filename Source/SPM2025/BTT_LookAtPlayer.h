// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_LookAtPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SPM2025_API UBTT_LookAtPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_LookAtPlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

