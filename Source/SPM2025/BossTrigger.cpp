// Fill out your copyright notice in the Description page of Project Settings.


#include "BossTrigger.h"
#include "DrawDebugHelpers.h"
#include "PlayerCharacter.h"
#define PRINT(text) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5,FColor::Green, FString(text))


ABossTrigger::ABossTrigger()
{
	OnActorBeginOverlap.AddDynamic(this, &ABossTrigger::OnEnterTrigger);
}

void ABossTrigger::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(
		GetWorld(),
		GetActorLocation(),
		GetComponentsBoundingBox().GetExtent(),
		FColor::Red,
		true,
		-1.f,
		0,
		5);
}
void ABossTrigger::OnEnterTrigger(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		if (auto* const Player = Cast<APlayerCharacter>(OtherActor))
		{
			PRINT(TEXT("Triggered"));
			OnBossTriggerActivated.Broadcast(PatrolPath);
		}
		
	}
}

