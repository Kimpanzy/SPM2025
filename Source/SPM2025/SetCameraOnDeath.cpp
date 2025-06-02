

#include "SetCameraOnDeath.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

ASetCameraOnDeath::ASetCameraOnDeath()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}

void ASetCameraOnDeath::BeginPlay()
{
	Super::BeginPlay();
		
	
}

void ASetCameraOnDeath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASetCameraOnDeath::OnDeath()
{
	
	if (!OurPlayerController)
	{
		OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	}

	if (OurPlayerController && NPC)
	{
		OurPlayerController->SetViewTargetWithBlend(NPC, 2.f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spelare finns inte"));
	}
}

