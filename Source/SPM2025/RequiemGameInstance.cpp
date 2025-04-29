// Fill out your copyright notice in the Description page of Project Settings.


#include "RequiemGameInstance.h"
#include "RequiemSaveGame.h"
#include "Saveable.h"
#include "Kismet/GameplayStatics.h"

URequiemGameInstance::URequiemGameInstance()
{
#if WITH_EDITOR
	LoadGame(false);
#endif
}

URequiemGameInstance* URequiemGameInstance::GetInstance(const UObject* WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);

#if WITH_EDITOR
	if (GameInstance->GetClass() != StaticClass())
	{
		UE_LOG(LogTemp, Fatal, TEXT("GameInstance not set to URequiemGameInstance!"));
	}
#endif

	return StaticCast<URequiemGameInstance*>(GameInstance);
}

void URequiemGameInstance::SaveGame(const bool Async)
{
	if (!SaveGameInstance)
	{
		CreateSaveGame();
	}

	if (Async)
	{
		FAsyncSaveGameToSlotDelegate OnSaveComplete;
		OnSaveComplete.BindLambda([&](const FString&, const int32, const bool Success)
		{
			SavedGame.ExecuteIfBound(SaveGameInstance, Success);
		});

		UGameplayStatics::AsyncSaveGameToSlot(
			SaveGameInstance,
			SaveGameInstance->GetSlotName(),
			SaveGameInstance->GetUserIndex(),
			OnSaveComplete
		);
	}
	else
	{
		const bool Success = UGameplayStatics::SaveGameToSlot(
			SaveGameInstance,
			SaveGameInstance->GetSlotName(),
			SaveGameInstance->GetUserIndex()
		);

		SavedGame.ExecuteIfBound(SaveGameInstance, Success);
	}
}

void URequiemGameInstance::LoadGame(const bool Async, const FString& SlotName, const int UserIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		if (Async)
		{
			FAsyncLoadGameFromSlotDelegate OnLoaded;
			OnLoaded.BindLambda([&](const FString&, const int32, USaveGame* SaveGame)
			{
				SaveGameInstance = Cast<URequiemSaveGame>(SaveGame);
				LoadedGame.ExecuteIfBound(SaveGameInstance);
			});

			UGameplayStatics::AsyncLoadGameFromSlot(SlotName, UserIndex, OnLoaded);
		}
		else
		{
			SaveGameInstance = Cast<URequiemSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
			LoadedGame.ExecuteIfBound(SaveGameInstance);
		}
	}
	else
	{
		CreateSaveGame(SlotName, UserIndex);
		LoadedGame.ExecuteIfBound(SaveGameInstance);
	}
}

void URequiemGameInstance::LoadGame(const bool Async)
{
	LoadGame(Async, TEXT("Slot0"), 0);
}

void URequiemGameInstance::RequestSave_Implementation(const bool Async)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveable::StaticClass(), Actors);

	TArray<TScriptInterface<ISaveable>> SaveableActors;
	SaveableActors.Reserve(Actors.Num());

	for (AActor*& Actor : Actors)
	{
		if (Actor->Implements<USaveable>())
		{
			SaveableActors.Emplace(TScriptInterface<ISaveable>(Actor));
		}
	}

	SaveGameInstance->SaveAllData(SaveableActors);
	SaveGame(Async);
}

void URequiemGameInstance::RequestLoad_Implementation(const TScriptInterface<ISaveable>& Saveable)
{
	SaveGameInstance->LoadDataFor(Saveable);
}

void URequiemGameInstance::CreateSaveGame(const FString& SlotName, const int UserIndex)
{
	CreateSaveGame();
	SaveGameInstance->SlotName = SlotName;
	SaveGameInstance->UserIndex = UserIndex;
}

void URequiemGameInstance::CreateSaveGame()
{
	SaveGameInstance = Cast<URequiemSaveGame>(UGameplayStatics::CreateSaveGameObject(URequiemSaveGame::StaticClass()));
}
