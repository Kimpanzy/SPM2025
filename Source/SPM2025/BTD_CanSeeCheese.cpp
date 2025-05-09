// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CanSeeCheese.h"
#include "RatAIController.h"
#include "VectorTypes.h"

UBTD_CanSeeCheese::UBTD_CanSeeCheese()
{
	NodeName = "Can See Cheese";
}

bool UBTD_CanSeeCheese::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ARatAIController* RatAICon = Cast<ARatAIController>(OwnerComp.GetAIOwner());
	
	if (!RatAICon)
	{
		return false;
	}

	const APawn* Rat = RatAICon->GetPawn();
	if(!Rat)
	{
		return false;
	}

	const int Range = RatAICon->bPlayerIsHoldingCheese ? 300 : 1000;
	return Rat->GetDistanceTo(RatAICon->GetCheese()) < Range && RatAICon->LineOfSightTo(RatAICon->GetCheese());
}
