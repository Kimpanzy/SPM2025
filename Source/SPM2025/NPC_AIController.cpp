// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_AIController.h"

#include "NPC.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
	
}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPC* const npc = Cast<ANPC>(InPawn))
	{
		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
			
		}
	}
}

void ANPC_AIController::SetupPerceptionSystem()
{
	/*SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(
			"Sight Component"));
		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(2.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.
		AddDynamic(this, &ANPC_AIController::OnTargetDetection);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}*/
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	if (HearingConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(
			"Hearing Component"));
		
		HearingConfig->HearingRange = 1000.0f;

		HearingConfig->SetMaxAge(.5f);
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		GetPerceptionComponent()->SetDominantSense(*HearingConfig->GetSenseImplementation());
		
		GetPerceptionComponent()->OnTargetPerceptionUpdated.
		AddDynamic(this, &ANPC_AIController::OnTargetDetection);
		
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
		
		
	}
}

void ANPC_AIController::OnTargetDetection(AActor* Actor, FAIStimulus Stimulus)
{
	
	if (auto* const Player = Cast<APlayerCharacter>(Actor))
	{
		//GetBlackboardComponent()->SetValueAsBool("CanSeePlayer",Stimulus.WasSuccessfullySensed());
		GetBlackboardComponent()->SetValueAsBool("CanHearPlayer",Stimulus.WasSuccessfullySensed());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", Player);
	}
	else if (Actor->ActorHasTag("Locker"))
	{
		FVector Location = Actor->GetActorLocation()+Actor->GetActorRightVector()*200+Actor->GetActorForwardVector()*20;
		GetBlackboardComponent()->SetValueAsVector("SoundLocation", Location);
		GetBlackboardComponent()->SetValueAsBool("IsHiding",true);
		UE_LOG(LogTemp, Warning, TEXT("HÖR Locker"));
	}else
	{
		GetBlackboardComponent()->SetValueAsVector("SoundLocation", Actor->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("HÖR FlashLight"));
	}
}