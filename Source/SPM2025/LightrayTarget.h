// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LightrayTarget.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class ULightrayTarget : public UInterface
{
	GENERATED_BODY()
};

class ILightrayTarget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Lightray")
	void OnRayHit();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Lightray")
	void OnRayStopHitting();
};
