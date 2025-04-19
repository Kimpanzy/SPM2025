// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class SPM2025_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();
	FVector GetPatrolPoint(int const Index) const;
	FVector setPatrolPoint(int const Index);
	int Num() const;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true", MakeEditWidget="true"))
	TArray<FVector> PatrolPoints;
};