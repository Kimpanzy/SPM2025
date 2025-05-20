// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBallGoal.h"

#include "Components/BoxComponent.h"


APushableBallGoal::APushableBallGoal()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APushableBallGoal::Complete()
{
	if (bIsComplete)
	{
		return;
	}

	bIsComplete = true;
	OnCompleted.Broadcast(this);
}

void APushableBallGoal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (bIsComplete || !OtherActor->ActorHasTag(TEXT("Ball")))
	{
		return;
	}

	OtherActor->DisableComponentsSimulatePhysics();
	OtherActor->SetActorLocation(TriggerBox->GetComponentLocation());
	Complete();
}

void APushableBallGoal::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APushableBallGoal::OnOverlap);
	}
}
