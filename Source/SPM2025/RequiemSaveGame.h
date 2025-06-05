// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallPushPuzzle.h"
#include "RequiemGameInstance.h"
#include "GameFramework/SaveGame.h"
#include "Mirror.h"
#include "NPC.h"
#include "PlayerCharacter.h"
#include "PushableBlock.h"
#include "PushPuzzle.h"
#include "RequiemSaveGame.generated.h"

class ISaveable;

USTRUCT(Blueprintable)
struct SPM2025_API FArtifactSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;
};

USTRUCT(Blueprintable)
struct SPM2025_API FRitualCircleSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> SlotObjectIds;
};

USTRUCT(Blueprintable)
struct SPM2025_API FRatCageSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bRatIsInCage = false;
};

USTRUCT(Blueprintable)
struct SPM2025_API FRatSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;
};

USTRUCT(Blueprintable)
struct SPM2025_API FLeverPuzzleSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<bool> PulledLevers;
};

USTRUCT(Blueprintable)
struct SPM2025_API FKeyDoorSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsOpen;
};

UCLASS()
class SPM2025_API URequiemSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FMirrorSaveData> Mirrors;

	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FPushableBlockSaveData> PushableBlocks;

	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FBallPushPuzzleSaveData> PushableBalls;

	UPROPERTY(VisibleAnywhere)
	FNPCSaveData NPC = FNPCSaveData();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, FArtifactSaveData> Artifacts;

	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FPushPuzzleSaveData> PushPuzzles;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, FLeverPuzzleSaveData> LeverPuzzles;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRitualCircleSaveData RitualCircle = FRitualCircleSaveData();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRatSaveData Rat = FRatSaveData();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRatCageSaveData RatCage = FRatCageSaveData();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FPlayerSaveData PlayerSaveData = FPlayerSaveData();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, FKeyDoorSaveData> KeyDoors;

private:
	friend void URequiemGameInstance::CreateSaveGame(const FString& SlotName, int UserIndex);

	UPROPERTY(VisibleAnywhere)
	FString SlotName = TEXT("Slot0");

	UPROPERTY(VisibleAnywhere)
	int UserIndex = 0;

public:
	FString GetSlotName() const { return SlotName; }
	int GetUserIndex() const { return UserIndex; }

	void SaveDataFor(const TScriptInterface<ISaveable>& SaveableActor);
	void SaveAllData(const TArray<TScriptInterface<ISaveable>>& SaveableActors);

	void LoadDataFor(const TScriptInterface<ISaveable>& SaveableActor);
};
