// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Grabber.generated.h"


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
	
	UFUNCTION(BlueprintCallable)
	void Release();
	UFUNCTION(BlueprintCallable)
	void Grab(FHitResult HitResult);
	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* GrabbedActor;
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	bool bCanDrop = false;

	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 400;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 30;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 100;

};