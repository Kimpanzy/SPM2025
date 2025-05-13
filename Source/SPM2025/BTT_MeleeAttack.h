// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTT_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SPM2025_API UBTT_MeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_MeleeAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
