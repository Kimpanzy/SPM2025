// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "PushableBlock.generated.h"

UCLASS()
class SPM2025_API APushableBlock : public AActor
{
	GENERATED_BODY()

public:
	APushableBlock();

private:
	FTimeline PushingTimeline;
	FVector PushingDirection = FVector::ZeroVector;
	FVector PushingFrom = FVector::ZeroVector;
	TWeakObjectPtr<ACharacter> Pusher;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), BlueprintReadWrite)
	UStaticMeshComponent* CubeMesh = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), BlueprintReadWrite)
	UCurveFloat* PushingCurve = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), BlueprintReadWrite)
	float PushLength = 100.f;

	bool CanPush(const ACharacter* Who, const FVector& Direction) const;
	void Push(ACharacter* Who, const FVector& Direction);

	UFUNCTION()
	void PushingUpdate(const float Alpha);

	UFUNCTION()
	void PushingFinished();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(const float DeltaTime) override;

	UFUNCTION()
	void OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	bool IsBeingPushed() const { return PushingTimeline.IsPlaying(); }
};
