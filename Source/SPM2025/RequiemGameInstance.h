// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Persistence.h"
#include "Engine/GameInstance.h"
#include "RequiemGameInstance.generated.h"

class ISaveable;
class URequiemSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSavedGameDelegate, URequiemSaveGame*, SaveGameInstance, bool, Success);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadGameDelegate, URequiemSaveGame*, SaveGameInstance);

UCLASS()
class SPM2025_API URequiemGameInstance : public UGameInstance, public IPersistence
{
	GENERATED_BODY()

public:
	static URequiemGameInstance* GetInstance(const UObject* WorldContextObject);

private:
	UPROPERTY()
	TObjectPtr<URequiemSaveGame> SaveGameInstance;

	void SaveGameToFile(bool Async);

public:
	UPROPERTY(BlueprintAssignable)
	FSavedGameDelegate SavedGame;

	UPROPERTY(BlueprintAssignable)
	FLoadGameDelegate LoadedGame;

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool Async = false, const FString& SlotName = TEXT("Slot0"), int UserIndex = 0);

	UFUNCTION(BlueprintCallable)
	void CreateSaveGame(const FString& SlotName = TEXT("Slot0"), int UserIndex = 0);

private:
	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void RequestSave_Implementation(bool Async) override;

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void RequestLoad_Implementation(const TScriptInterface<ISaveable>& Saveable) override;
};
