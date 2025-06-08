// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Saveable.h"
#include "Mirror.generated.h"

UCLASS()
class SPM2025_API AMirror : public AActor, public IInteractable, public ISaveable
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
	UPROPERTY(VisibleAnywhere, NonPIEDuplicateTransient)
	FGuid ID = FGuid::NewGuid();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator RotationAngle = FRotator(0, 90, 0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCurveFloat* RotationCurve = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UMeshComponent* MirrorMeshComponent = nullptr;

	virtual void Tick(const float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Rotate();

private:
	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void OnInteract_Implementation(FHitResult HitResult) override { Rotate(); }

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	FText GetInteractPrompt_Implementation(FHitResult HitResult) override { return FText::FromString(TEXT("Rotate")); }

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void SaveData_Implementation(URequiemSaveGame* SaveGameInstance) override;

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void LoadData_Implementation(URequiemSaveGame* SaveGameInstance) override;
};

USTRUCT()
struct SPM2025_API FMirrorSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FRotator Rotation = FRotator::ZeroRotator;
};
