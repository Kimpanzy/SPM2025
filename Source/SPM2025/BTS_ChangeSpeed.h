// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class SPM2025_API UBTS_ChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:
	UBTS_ChangeSpeed();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess=true))
	float Speed = 600.f;
	
};
