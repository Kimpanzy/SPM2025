// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MeleeAttack.h"

#include "AIController.h"
#include "NPC.h"
#include "Kismet/GameplayStatics.h"

UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Kollar NPC är out of range och attackerar inte
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	//Hämta AI controller och AI
	auto* const Cont = OwnerComp.GetAIOwner();
	auto* const NPC = Cast<ANPC>(Cont->GetPawn());
	bool const Hidding = OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsHiding");
	auto* const icombat = Cast<ICombatInterface>(NPC);
	
	//Kollar så Ai har implementerat interfacet
	//tog bort hiding checken här - herman
	if (auto* const PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		if ( auto* const Player = Cast<APlayerCharacter>(PlayerChar))
		{
			
			if ( icombat && !Player->bIsHiding)
			{
				icombat->Execute_MeleeAttack(NPC);
			}
		}
	}
	

	//Avlusta med success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

