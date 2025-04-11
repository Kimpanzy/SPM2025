// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flashlight.generated.h"

class USpotLightComponent;
UCLASS()
class SPM2025_API AFlashlight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlashlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ToggleFlashlight();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* OuterSpotLight;
	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* InnerSpotLight;
	UPROPERTY(VisibleAnywhere)
	float Battery = 100.0f;

	UPROPERTY(EditAnywhere)
	bool bIsOn;

};
