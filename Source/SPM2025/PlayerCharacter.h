// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class AFlashlight;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFlashLightDelegate);

UCLASS()
class SPM2025_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

	//-->Input actions<--
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Enhanced Input")
	UInputMappingContext*  PlayerInputContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Enhanced Input")
	UInputAction* FlashlightAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Enhanced Input")
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Enhanced Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Enhanced Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Enhanced Input")
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	void InputMove(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputLook(const FInputActionValue& Value);
	void InputSprint(const FInputActionValue& Value);

public:	
	// Called every frameees
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Flashlight)
	TSubclassOf<AFlashlight> FlashlightClass;
	TSoftObjectPtr<AFlashlight> Flashlight;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleFlashlight(const FInputActionValue& value);

	UPROPERTY(BlueprintAssignable)
	FFlashLightDelegate OnFlashlightToggled;
	

};
