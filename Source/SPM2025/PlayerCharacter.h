// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NPC_AIController.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class AFlashlight;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFlashLightDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDied);

UCLASS()
class SPM2025_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Flashlight)
	TSubclassOf<AFlashlight> FlashlightClass;
	UPROPERTY(BlueprintReadOnly, category = Flashlight)
	AFlashlight* Flashlight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ANPC_AIController* AIController;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void ToggleFlashlight(const FInputActionValue& value);

	UPROPERTY(BlueprintAssignable)
	FFlashLightDelegate OnFlashlightToggled;
	UPROPERTY(BlueprintAssignable)
	FPlayerDied OnPlayerDeath;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* ArmSkeleton;

	void InputMove(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputLook(const FInputActionValue& Value);
	void InputSprint(const FInputActionValue& Value);
	
private:
	//Så AI kan höra/se spelare
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();
	
	
	

};
