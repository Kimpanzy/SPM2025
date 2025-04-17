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

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere,Category="Mesh")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere,Category="Light")
	USpotLightComponent* OuterSpotLight;
	UPROPERTY(VisibleAnywhere,Category="Light")
	USpotLightComponent* InnerSpotLight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void ToggleFlashlight();


	UPROPERTY(BlueprintReadWrite)
	bool bIsOn;
	UPROPERTY(BlueprintReadWrite)
	bool bCanLight = true;
	
	UFUNCTION(BlueprintCallable)
	void FlickerLight(bool OnOrOff);
	
	UFUNCTION(BlueprintImplementableEvent)
	void Flicker();

};
