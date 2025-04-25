// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_IsPlayerInMeleeRange.h"

#include "NPC.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTS_IsPlayerInMeleeRange::UBTS_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTS_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Hämta controller och AI
	auto* const Cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	auto* const NPC = Cast<ANPC>(Cont->GetPawn());

	//Hämta spelar karaktären
	auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);

	//Skriv till blackboard bool true / false om spelare är i range
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(),
		NPC->GetDistanceTo(Player) <= MeleeRange);
	
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}
