// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PatrolPath.h"
#include "CombatInterface.h"
#include "Saveable.h"
#include "PlayerCharacter.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Sound/SoundAttenuation.h"
#include "NPC.generated.h"

UCLASS()
class SPM2025_API ANPC : public ACharacter, public ICombatInterface, public ISaveable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;

	APatrolPath* GetPatrolPath() const;
	void SetPatrolPath(APatrolPath* path);

	USoundBase* GetSound() const;

	UAudioComponent* GetAudioComponent() const;

	USoundAttenuation* GetSoundAttenuation() const;

	UFUNCTION(BlueprintCallable)
	void UnlockPath(APatrolPath* Path);
	UFUNCTION(BlueprintCallable)
	void LockPath(APatrolPath* Path);

	void PlayWalkingSound();

	virtual int MeleeAttack_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TArray<APatrolPath*> AllPaths;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TArray<APatrolPath*> UnlockedPaths;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle FootstepTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	APlayerCharacter* Player;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float StepInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* Sound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* FootstepAudioComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* ActiveSoundComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* WalkingSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundAttenuation* ATTSound;

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void SaveData_Implementation(URequiemSaveGame* SaveGameInstance) override;

	// ReSharper disable once CppEnforceOverridingFunctionStyle - Not Required for Unreal Interface
	void LoadData_Implementation(URequiemSaveGame* SaveGameInstance) override;
};

USTRUCT()
struct SPM2025_API FNPCSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> UnlockedPaths;
};
