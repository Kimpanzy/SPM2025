// Fill out your copyright notice in the Description page of Project Settings.


#include "Mirror.h"

#include "RequiemGameInstance.h"
#include "RequiemSaveGame.h"
#include "Kismet/KismetMathLibrary.h"

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

	URequiemGameInstance::Execute_RequestLoad(URequiemGameInstance::GetInstance(GetWorld()), this);
}

void AMirror::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotationTimeline.TickTimeline(DeltaTime);

#if WITH_EDITOR
	if (MirrorMeshComponent && !GetWorld()->IsPlayInEditor())
	{
		const FVector Location = MirrorMeshComponent->GetComponentLocation();
		const FVector MirrorForwardVector = UKismetMathLibrary::GetForwardVector(MirrorMeshComponent->GetComponentRotation());
		DrawDebugLine(GetWorld(), Location, Location + MirrorForwardVector * 300, {0, 255, 255}, false, .2f);
	}
#endif
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

void AMirror::SaveData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	SaveGameInstance->Mirrors.Add(
		ID,
		FMirrorSaveData{
			RotationTimeline.IsPlaying() ? RotatingTo : GetActorRotation()
		}
	);
}

void AMirror::LoadData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	if (const FMirrorSaveData* SaveData = SaveGameInstance->Mirrors.Find(ID))
	{
		SetActorRotation(SaveData->Rotation);
	}
}
