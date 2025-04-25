// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Mirror.generated.h"

UCLASS()
class SPM2025_API AMirror : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AMirror();

private:
	FRotator RotatingTo;
	FRotator RotatingFrom;

	FTimeline RotationTimeline;

	UFUNCTION()
	void RotationUpdate(const float Alpha);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Mirror")
	FRotator RotationAngle = FRotator(0, 90, 0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Mirror")
	UCurveFloat* RotationCurve = nullptr;

	virtual void Tick(const float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Mirror")
	void Rotate();

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void OnInteract_Implementation(FHitResult HitResult) override { Rotate(); };

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	FText GetInteractPrompt_Implementation(FHitResult HitResult) override { return FText::FromString(TEXT("Rotate")); };
};
