// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBallGoal.h"

#include "PushBall.h"
#include "Components/BoxComponent.h"


APushableBallGoal::APushableBallGoal()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APushableBallGoal::Complete(APushBall* BallInGoal)
{
	if (bIsComplete)
	{
		return;
	}

	BallInGoal->DisableComponentsSimulatePhysics();
	BallInGoal->SetActorLocation(TriggerBox->GetComponentLocation());
	BallInGoal->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	bIsComplete = true;
	Ball = BallInGoal;
	OnCompleted.Broadcast(this);
}

void APushableBallGoal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (bIsComplete)
	{
		return;
	}

	if (APushBall* OtherBall = Cast<APushBall>(OtherActor))
	{
		Complete(OtherBall);
	}
}

void APushableBallGoal::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APushableBallGoal::OnOverlap);
	}
}
