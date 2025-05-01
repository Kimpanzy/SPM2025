// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MakeSound.h"

#include "NPC.h"
#include "NPC_AIController.h"
#include "Kismet/GameplayStatics.h"

UBTT_MakeSound::UBTT_MakeSound(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "AI Scream";
}

EBTNodeResult::Type UBTT_MakeSound::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const NPC = Cast<ANPC>(cont->GetPawn()))
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				NPC->GetSound(),
				NPC->GetActorLocation(),
				0.4f,
				1,
				0,
				NPC->GetSoundAttenuation()
				);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
