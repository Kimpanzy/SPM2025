// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Persistence.h"
#include "Engine/GameInstance.h"
#include "RequiemGameInstance.generated.h"

class ISaveable;
class URequiemSaveGame;

DECLARE_DELEGATE_TwoParams(FSavedGameDelegate, URequiemSaveGame* SaveGameInstance, bool Success);
DECLARE_DELEGATE_OneParam(FLoadGameDelegate, URequiemSaveGame* SaveGameInstance);

UCLASS()
class SPM2025_API URequiemGameInstance : public UGameInstance, public IPersistence
{
	GENERATED_BODY()

public:
	URequiemGameInstance();

	static URequiemGameInstance* GetInstance(const UObject* WorldContextObject);

private:
	UPROPERTY()
	TObjectPtr<URequiemSaveGame> SaveGameInstance;

	void SaveGame(bool Async);

public:
	FSavedGameDelegate SavedGame;
	FLoadGameDelegate LoadedGame;

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void RequestSave_Implementation(bool Async) override;

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void RequestLoad_Implementation(const TScriptInterface<ISaveable>& Saveable) override;

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool Async);
	void LoadGame(bool Async, const FString& SlotName, int UserIndex);

	UFUNCTION(BlueprintCallable)
	void CreateSaveGame();
	void CreateSaveGame(const FString& SlotName, int UserIndex);
};
