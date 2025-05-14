// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC.h"
#include "RequiemSaveGame.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepAudioComponent"));
	FootstepAudioComponent->bAutoActivate = false;
	FootstepAudioComponent->SetupAttachment(RootComponent);
	ActiveSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ActiveSound"));
	ActiveSoundComponent->bAutoActivate = false;
	ActiveSoundComponent->SetupAttachment(RootComponent);
	

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundPaths;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPath::StaticClass(), FoundPaths);

	for (auto* Actor : FoundPaths)
	{
		APatrolPath* Path = Cast<APatrolPath>(Actor);
		if (Path)
		{
			AllPaths.Add(Path);
			//UnlockedPaths.Add(Path);
		}
	}
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (WalkingSound)
	{
		FootstepAudioComponent->SetSound(WalkingSound);
		PlayWalkingSound();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WalkingSound is not set!"));
	}
}

void ANPC::SaveData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	FNPCSaveData SaveData;
	
	for (auto* Path : AllPaths)
	{
		FPatrolPoints Points;
		
		Points.PatrolPoints = Path->PatrolPoints;

		SaveData.AllPaths.Add(Points);
	}
	
	
}

void ANPC::LoadData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	
	FNPCSaveData SaveData = SaveGameInstance->NPC;
	TArray<FPatrolPoints> Path = SaveData.AllPaths;
	
	for (int i = 0; i < AllPaths.Num(); i++)
	{
		if (i < Path.Num())
		{
			AllPaths[i]->PatrolPoints = Path[i].PatrolPoints;
		}
	}
	
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
void ANPC::PlayWalkingSound()
{
		FootstepAudioComponent->Play();
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

USoundBase* ANPC::GetSound() const
{
	return Sound;
}

UAudioComponent* ANPC::GetAudioComponent() const
{
	return ActiveSoundComponent;
}

USoundAttenuation* ANPC::GetSoundAttenuation() const
{
	return ATTSound;
}

void ANPC::UnlockPath(APatrolPath* Path)
{
	if (Path && !UnlockedPaths.Contains(Path))
	{
		UnlockedPaths.Add(Path);
	}
}

void ANPC::LockPath(APatrolPath* Path)
{
	if (Path && UnlockedPaths.Contains(Path))
	{
		UnlockedPaths.Remove(Path);
	}
}


int ANPC::MeleeAttack_Implementation()
{
	if (Player)
	{
		Player->OnPlayerDeath.Broadcast(this);
		UE_LOG(LogTemp, Warning, TEXT("ATTACKING!"));
		
	}
	return 0;
}

