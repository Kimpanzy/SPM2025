// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"

#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"

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
	OuterSpotLight->SetupAttachment(Mesh);
	OuterSpotLight->SetOuterConeAngle(30.0f);

	InnerSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("InnerSpotLight"));
	InnerSpotLight->SetupAttachment(Mesh);
	InnerSpotLight->SetOuterConeAngle(10.0f);

	bIsOn = false;
}

void AFlashlight::ToggleFlashlight()
{
	if (!bCanLight)
	{
		return;
	}
	bIsOn = !bIsOn;
	FlickerLight(bIsOn);
	UGameplayStatics::PlaySound2D(GetWorld(), ToggleFlashLightSFX);
	if (bIsOn)
	{
		Flicker();
	}
	//InnerSpotLight->SetVisibility(bIsOn);
	//Mesh->SetVisibility(bIsOn);
}

void AFlashlight::FlickerLight(bool OnOrOff)
{
	OuterSpotLight->SetVisibility(OnOrOff);
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

