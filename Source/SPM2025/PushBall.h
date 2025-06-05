// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PushBall.generated.h"

UCLASS()
class SPM2025_API APushBall : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, NonPIEDuplicateTransient, BlueprintReadOnly)
	FGuid ID = FGuid::NewGuid();
	
};
