// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_CanSeeCheese.generated.h"

/**
 * 
 */
UCLASS()
class SPM2025_API UBTD_CanSeeCheese : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTD_CanSeeCheese();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
