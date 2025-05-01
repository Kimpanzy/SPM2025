// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MakeSound.generated.h"

/**
 * 
 */
UCLASS()
class SPM2025_API UBTT_MakeSound : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	explicit UBTT_MakeSound(const FObjectInitializer& ObjectInitializer);

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
