// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyDoor.h"

#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"

// Sets default values
AKeyDoor::AKeyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AKeyDoor::EnableNavMesh()
{
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	for (UStaticMeshComponent* Comp : StaticMeshComponents)
	{
		Comp->SetCanEverAffectNavigation(false);
	}
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		NavSys->Build();
	}
}

// Called when the game starts or when spawned
void AKeyDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

