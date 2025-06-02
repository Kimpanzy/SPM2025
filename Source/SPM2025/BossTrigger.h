// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPath.h"
#include "Engine/TriggerBox.h"
#include "BossTrigger.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossTriggerActivated, APatrolPath*, PatrolRoute);

UCLASS()
class SPM2025_API ABossTrigger : public ATriggerBox
{
	GENERATED_BODY()
public:
	
	ABossTrigger();

	UPROPERTY(EditInstanceOnly, Category = "Patrol")
	APatrolPath* PatrolPath;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBossTriggerActivated OnBossTriggerActivated;
	
	UPROPERTY(EditInstanceOnly, Category = "AISound")
	USoundBase* Sound;
	UPROPERTY(EditAnywhere, Category = "AISound")
	USoundAttenuation* SoundAttenuation;
protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnEnterTrigger(AActor* OverlappedActor, AActor* OtherActor);
};
