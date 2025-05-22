// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_PlayerIsClose.h"

#include "RatAIController.h"

UBTD_PlayerIsClose::UBTD_PlayerIsClose()
{
	NodeName = "Player Is Close";
}

bool UBTD_PlayerIsClose::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ARatAIController* RatAICon = Cast<ARatAIController>(OwnerComp.GetAIOwner());
	if (!RatAICon)
	{
		return false;
	}
	auto* const RatPawn = RatAICon->GetPawn();
	
	return RatPawn && RatPawn->GetDistanceTo(RatAICon->Player) < 500 && !RatAICon->bPlayerIsHoldingCheese;
}
