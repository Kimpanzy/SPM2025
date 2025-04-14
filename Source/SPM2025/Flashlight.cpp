// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"

#include "Components/SpotLightComponent.h"

// Sets default values
AFlashlight::AFlashlight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	OuterSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("OuterSpotLight"));
	OuterSpotLight->SetupAttachment(RootComponent);
	OuterSpotLight->SetIntensity(500.0f);
	OuterSpotLight->SetAttenuationRadius(500.0f);
	OuterSpotLight->SetOuterConeAngle(30.0f);

	InnerSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("InnerSpotLight"));
	InnerSpotLight->SetupAttachment(RootComponent);
	InnerSpotLight->SetIntensity(750.0f);
	InnerSpotLight->SetAttenuationRadius(500.0f);
	InnerSpotLight->SetOuterConeAngle(10.0f);

	bIsOn = false;
}

void AFlashlight::ToggleFlashlight()
{
	UE_LOG(LogTemp, Display, TEXT("Flashlight is toggling"));
	bIsOn = !bIsOn;
	OuterSpotLight->SetVisibility(bIsOn);
	InnerSpotLight->SetVisibility(bIsOn);
	Mesh->SetVisibility(bIsOn);
}

// Called when the game starts or when spawned
void AFlashlight::BeginPlay()
{
	Super::BeginPlay();
	OuterSpotLight->SetVisibility(bIsOn);
	InnerSpotLight->SetVisibility(bIsOn);
}

// Called every frame
void AFlashlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

