// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Persistence.generated.h"

class ISaveable;

UINTERFACE()
class UPersistence : public UInterface
{
	GENERATED_BODY()
};

class SPM2025_API IPersistence
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RequestSave(bool Async);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RequestLoad(const TScriptInterface<ISaveable>& Saveable);
};
