// Fill out your copyright notice in the Description page of Project Settings.


#include "RequiemSaveGame.h"
#include "Saveable.h"

void URequiemSaveGame::SaveDataFor(const TScriptInterface<ISaveable>& SaveableActor)
{
	ISaveable::Execute_SaveData(SaveableActor.GetObject(), this);
}

void URequiemSaveGame::SaveAllData(const TArray<TScriptInterface<ISaveable>>& SaveableActors)
{
	for (TScriptInterface Saveable : SaveableActors)
	{
		SaveDataFor(Saveable);
	}
}

void URequiemSaveGame::LoadDataFor(const TScriptInterface<ISaveable>& SaveableActor)
{
	ISaveable::Execute_LoadData(SaveableActor.GetObject(), this);
}
