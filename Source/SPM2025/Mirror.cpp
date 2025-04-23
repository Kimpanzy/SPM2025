// Fill out your copyright notice in the Description page of Project Settings.


#include "Mirror.h"

AMirror::AMirror(): RotatingTo(), RotatingFrom()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMirror::RotationUpdate(const float Alpha)
{
	SetActorRotation(FMath::Lerp(RotatingFrom, RotatingTo, Alpha));
}

void AMirror::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat RotationUpdateEvent;
	RotationUpdateEvent.BindDynamic(this, &AMirror::RotationUpdate);

	RotationTimeline.AddInterpFloat(RotationCurve, RotationUpdateEvent);
}

void AMirror::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotationTimeline.TickTimeline(DeltaTime);
}

void AMirror::Rotate()
{
	if (RotationTimeline.IsPlaying())
	{
		return;
	}

	RotatingFrom = GetActorRotation();
	RotatingTo = RotatingFrom + RotationAngle;

	RotationTimeline.PlayFromStart();
}
