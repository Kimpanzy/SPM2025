// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Flashlight.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetCapsuleComponent())
	{
		SetRootComponent(GetCapsuleComponent());
	}
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	if (CameraComponent)
	{
		CameraComponent->SetupAttachment(RootComponent);
		CameraComponent->bUsePawnControlRotation = true;
	}
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(PlayerInputContext, 0);
		}
	}
	Flashlight = GetWorld()->SpawnActor<AFlashlight>(AFlashlight::StaticClass());

	if (Flashlight)
	{
		UE_LOG(LogTemp, Warning, TEXT("Flashlight spawned successfully!"));

		// Attach to the camera
		Flashlight->AttachToComponent(CameraComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Flashlight->SetActorRelativeLocation(FVector(30.0f, 0.0f, 10.0f));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn flashlight!"));
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(FlashlightAction,ETriggerEvent::Started, this, &APlayerCharacter::ToggleFlashlight);
	}


	
	//InputComponent->BindAction("ToggleFlashlight", IE_Pressed, this, &APlayerCharacter::ToggleFlashlight());

}

void APlayerCharacter::ToggleFlashlight(const FInputActionValue& Value)
{
	
	Flashlight->ToggleFlashlight();
}

