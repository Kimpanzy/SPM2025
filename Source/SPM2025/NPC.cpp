// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Kismet/GameplayStatics.h"
#include "LevelInstance/LevelInstanceTypes.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	return Tree;
}

APatrolPath* ANPC::GetPatrolPath() const
{
	return PatrolPath;
}

void ANPC::SetPatrolPath(APatrolPath* path)
{
	PatrolPath = path;
}

UAnimMontage* ANPC::GetMontage() const
{
	return Montage;
}

USoundBase* ANPC::GetSound() const
{
	return Sound;
}

USoundAttenuation* ANPC::GetSoundAttenuation() const
{
	return ATTSound;
}

int ANPC::MeleeAttack_Implementation()
{
	Player->OnPlayerDeath.Broadcast();
	UE_LOG(LogTemp, Warning,TEXT("ATTACKING!"))
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
	return 0;
}

