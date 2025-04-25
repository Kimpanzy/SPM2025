// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_IsPlayerInMeleeRange.generated.h"

/**
 * 
 */
UCLASS()
class SPM2025_API UBTS_IsPlayerInMeleeRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_IsPlayerInMeleeRange();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Ai", meta=(AllowPrivateAccess=true))
	float MeleeRange = 2.5f;
};
