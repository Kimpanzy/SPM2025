
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SetCameraOnDeath.generated.h"

UCLASS()
class SPM2025_API ASetCameraOnDeath : public AActor
{
	GENERATED_BODY()
	
public:	
	ASetCameraOnDeath();

	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnDeath();
	
	UPROPERTY(EditAnywhere)
	APawn* NPC = nullptr;
	UPROPERTY()
	APlayerController* OurPlayerController = nullptr;
protected:
	virtual void BeginPlay() override;



};
