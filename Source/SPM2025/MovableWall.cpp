// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableWall.h"

#include "Kismet/GameplayStatics.h"


AMovableWall::AMovableWall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMovableWall::MovingUpdate(const float Alpha)
{
	SetActorLocation(MoveOffset * Alpha + MovingFrom);
}

void AMovableWall::MovingFinished()
{
	SetActorTickEnabled(false);
}

void AMovableWall::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat PushingUpdateEvent;
	PushingUpdateEvent.BindDynamic(this, &AMovableWall::MovingUpdate);
	MovingTimeline.AddInterpFloat(PushingCurve, PushingUpdateEvent);

	FOnTimelineEvent PushingFinishEvent;
	PushingFinishEvent.BindDynamic(this, &AMovableWall::MovingFinished);
	MovingTimeline.SetTimelineFinishedFunc(PushingFinishEvent);
}

void AMovableWall::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovingTimeline.TickTimeline(DeltaTime);
}

void AMovableWall::Move()
{
	if (bHasMoved)
	{
		return;
	}

	bHasMoved = true;
	MovingFrom = GetActorLocation();
	MovingTimeline.PlayFromStart();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), MovingSound, GetActorLocation());
}
