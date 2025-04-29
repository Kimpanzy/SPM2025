// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RequiemGameInstance.h"
#include "GameFramework/SaveGame.h"
#include "Mirror.h"
#include "RequiemSaveGame.generated.h"

class ISaveable;

UCLASS()
class SPM2025_API URequiemSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FMirrorSaveData> Mirrors;

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
