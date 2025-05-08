// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MovableWall.generated.h"

UCLASS()
class SPM2025_API AMovableWall : public AActor
{
	GENERATED_BODY()

public:
	AMovableWall();

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), BlueprintReadWrite)
	FVector MoveOffset = {0, 0, 300};

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true), BlueprintReadWrite)
	UCurveFloat* PushingCurve = nullptr;

	FTimeline MovingTimeline;
	bool bHasMoved = false;
	FVector MovingFrom{};

	UFUNCTION()
	void MovingUpdate(const float Alpha);

	UFUNCTION()
	void MovingFinished();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Move();
};
