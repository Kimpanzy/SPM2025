// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_ChangeSpeed.h"

#include "NPC.h"
#include "NPC_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTS_ChangeSpeed::UBTS_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
}

void UBTS_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (auto* const Cont = OwnerComp.GetAIOwner())
	{
		if (auto* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}
