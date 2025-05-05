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
	int range = 1000; 
	if (RatAICon->bPlayerIsHoldingCheese)
	{
		range = 300;
	}
	return RatAICon->GetPawn()->GetDistanceTo(RatAICon->GetCheese()) < range &&RatAICon->LineOfSightTo(RatAICon->GetCheese());
} 
