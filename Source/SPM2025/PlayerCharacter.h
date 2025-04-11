// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class AFlashlight;
class UCameraComponent;

UCLASS()
class SPM2025_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Input)
	UInputMappingContext*  PlayerInputContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Input)
	UInputAction* FlashlightAction;
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Flashlight)
	AFlashlight* Flashlight;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleFlashlight(const FInputActionValue& value);

};
