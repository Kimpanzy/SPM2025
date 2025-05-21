// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Flashlight.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "RequiemSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetCapsuleComponent())
	{
		SetRootComponent(GetCapsuleComponent());
	}
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArm);

	ArmSkeleton = CreateDefaultSubobject<USkeletalMeshComponent>("ArmSkeleton");
	ArmSkeleton->SetupAttachment(SpringArm);

	CameraComponent->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 1.0f;

	SetupStimulusSource();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(PlayerInputContext, 0);
		}
	}
	//Spawn flashlight
	Flashlight = GetWorld()->SpawnActor<AFlashlight>(FlashlightClass);
	//Attach flashlight till kameran
	if (Flashlight)
	{
		Flashlight->AttachToComponent(ArmSkeleton, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_L");
		//Flashlight->AttachToComponent(CameraComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Flashlight->SetActorRelativeLocation(FVector(0.04, 0.06, 0.22));
		Flashlight->SetActorRelativeRotation(FRotator(11, 270, 50));
		Flashlight->SetActorRelativeScale3D(FVector(0.004));
	}
	Super::BeginPlay();

	URequiemGameInstance::Execute_RequestLoad(URequiemGameInstance::GetInstance(GetWorld()), this);
	OnPlayerDeath.AddDynamic(this, &APlayerCharacter::HandlePlayerDeath);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RaycastHighligh();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::ToggleFlashlight);
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::InputMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputLook);
	}
}

void APlayerCharacter::InputMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void InputSprint(const FInputActionValue& Value)
{
}

void APlayerCharacter::InputLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X * 0.34f);
		AddControllerPitchInput(LookAxisValue.Y * 0.34f);
	}
}

void APlayerCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("Stimulus");
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void APlayerCharacter::ToggleFlashlight(const FInputActionValue& Value)
{
	OnFlashlightToggled.Broadcast();
	Flashlight->ToggleFlashlight();
}

void APlayerCharacter::RaycastHighligh()
{
	FVector Start = CameraComponent->GetComponentLocation();
	FVector Forward = CameraComponent->GetForwardVector();
	FVector End = Start + (Forward * 250.f);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(Hit, Start, End,FQuat::Identity, ECC_Visibility,FCollisionShape::MakeSphere(10), QueryParams);

	if (LastHighlightedActor && LastHighlightedActor != Hit.GetActor())
	{
		TArray<UStaticMeshComponent*> PrevMeshComponents;
		LastHighlightedActor->GetComponents<UStaticMeshComponent>(PrevMeshComponents);
		for (auto* Comp : PrevMeshComponents)
		{
			Comp->SetRenderCustomDepth(false);
		}
		LastHighlightedActor = nullptr;
	}

	if (bHit && Hit.GetActor() && Hit.GetActor()->ActorHasTag("Highlight"))
	{
		LastHighlightedActor = Hit.GetActor();
		TArray<UStaticMeshComponent*> MeshComponents;
		Hit.GetActor()->GetComponents<UStaticMeshComponent>(MeshComponents);
		for (auto* MeshComp : MeshComponents)
		{
			MeshComp->SetRenderCustomDepth(true);
		}
	}
}

void APlayerCharacter::HandlePlayerDeath(AActor* Killer)
{
	
	if (bIsDead)
	{
		return;
	}
	bIsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("HandlePlayer!"));
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && Killer)
	{
		PC->SetViewTargetWithBlend(Killer, 0.2f);
	}

	DisableInput(PC);
	if (auto* const Player = GetCharacterMovement())
	{
		Player->DisableMovement();
		Player->StopMovementImmediately();
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (Flashlight)
	{
		Flashlight->SetActorHiddenInGame(true);
		Flashlight->SetActorEnableCollision(false);
	}

	if (StimulusSource)
	{
		StimulusSource->UnregisterFromPerceptionSystem();
	}
	SetActorTickEnabled(false);
}
