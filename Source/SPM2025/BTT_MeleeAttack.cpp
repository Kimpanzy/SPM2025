// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MeleeAttack.h"

#include "AIController.h"
#include "NPC.h"

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
	

	//Kollar så Ai har implementerat interfacet
	if (auto* const icombat = Cast<ICombatInterface>(NPC))
	{
		icombat->Execute_MeleeAttack(NPC);
	}

	//Avlusta med success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTT_MeleeAttack::MontageHasFinished(ANPC* const NPC)
{
	return NPC->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(NPC->GetMontage());
}
