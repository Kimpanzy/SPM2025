// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MakeSound.h"

#include "NPC.h"
#include "NPC_AIController.h"
#include "Components/AudioComponent.h"
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
			UAudioComponent* AC = NPC->GetAudioComponent();
			if ( AC && AC->IsPlaying())
			{
				return EBTNodeResult::Succeeded;
			}
			AC->Sound = NPC->GetSound();
			AC->AttenuationSettings = NPC->GetSoundAttenuation();
			AC->VolumeMultiplier = 0.3f;
			AC->Play();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
