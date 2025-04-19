// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"
// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	for (const FVector& Point : PatrolPoints)
	{
		// Change color/size as needed
		DrawDebugSphere(
			GetWorld(),
			Point,
			20.0f, // radius
			12,    // segments
			FColor::Green,
			true,  // persistent (will stay until you close editor or rebuild)
			-1.0f  // lifetime (-1 = forever)
		);
	}
}

FVector APatrolPath::GetPatrolPoint(int const Index) const
{
	return PatrolPoints[Index];
}

FVector APatrolPath::setPatrolPoint(int const Index)
{
	return PatrolPoints[Index];
}

int APatrolPath::Num() const
{
	return PatrolPoints.Num();
}


