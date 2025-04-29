// Fill out your copyright notice in the Description page of Project Settings.


#include "RequiemSaveGame.h"
#include "Saveable.h"

void URequiemSaveGame::SaveAllData(const TArray<TScriptInterface<ISaveable>>& SaveableActors)
{
	for (TScriptInterface Saveable : SaveableActors)
	{
		ISaveable::Execute_SaveData(Saveable.GetObjectRef(), this);
	}
}

void URequiemSaveGame::LoadDataFor(TScriptInterface<ISaveable> SaveableActor)
{
	ISaveable::Execute_LoadData(SaveableActor.GetObjectRef(), this);
}
