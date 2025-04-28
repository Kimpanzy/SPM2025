
#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Grabber.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemGrabbed, AActor*, GrabbedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemDropped, AActor*, DroppedItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class SPM2025_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void Release();
	UFUNCTION(BlueprintCallable)
	void ReleaseAtPos(FVector Location, FRotator Rotation);
	UFUNCTION(BlueprintCallable)
	void Grab(AActor* HitActor, FVector LocationOffset, FRotator RotationOffset);
	UPROPERTY(BlueprintReadWrite)
	AActor* GrabbedActor;
	UPROPERTY(BlueprintAssignable)
	FItemGrabbed ItemGrabbed;
	UPROPERTY(BlueprintAssignable)
	FItemDropped ItemDropped;
	
	
private:

	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 400;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 30;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 100;

	UCameraComponent* Camera;

	FRotator RotationOffset;
	FVector LocationOffset;

};