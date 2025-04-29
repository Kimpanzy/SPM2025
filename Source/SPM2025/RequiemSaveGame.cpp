// Fill out your copyright notice in the Description page of Project Settings.


#include "RequiemSaveGame.h"
#include "Saveable.h"

void URequiemSaveGame::SaveDataFor(TScriptInterface<ISaveable>& SaveableActor)
{
	ISaveable::Execute_SaveData(SaveableActor.GetObjectRef(), this);
}

void URequiemSaveGame::SaveAllData(const TArray<TScriptInterface<ISaveable>>& SaveableActors)
{
	for (TScriptInterface Saveable : SaveableActors)
	{
		SaveDataFor(Saveable);
	}
}

void URequiemSaveGame::LoadDataFor(TScriptInterface<ISaveable> SaveableActor)
{
	ISaveable::Execute_LoadData(SaveableActor.GetObjectRef(), this);
}
