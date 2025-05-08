// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBlock.h"

#include "PushPuzzleGoal.h"
#include "RequiemGameInstance.h"
#include "RequiemSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

APushableBlock::APushableBlock()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APushableBlock::MovingUpdate(const float Alpha)
{
	SetActorRelativeLocation(PushingDirection * PushLength * Alpha + PushingFrom);
}

void APushableBlock::MovingFinished()
{
	if (!bIsPushingEnabled)
	{
		return;
	}

	if (Pusher.IsValid() && Pusher->IsValidLowLevel())
	{
		Pusher->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Pusher->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	Pusher.Reset();

	const FVector ActorLocation = GetActorLocation();

	const FCollisionObjectQueryParams CollisionObjectParams(
		ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic));

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult,
	                                        ActorLocation,
	                                        FVector::DownVector * PushLength + ActorLocation,
	                                        CollisionObjectParams
	);

	if (APushPuzzleGoal* HitGoal = Cast<APushPuzzleGoal>(HitResult.GetActor()))
	{
		HitGoal->Complete();

		bIsPushingEnabled = false;
		PushingFrom = ActorLocation;
		PushingDirection = FVector::DownVector;
		MovingTimeline.PlayFromStart();
	}
}

void APushableBlock::BeginPlay()
{
	Super::BeginPlay();

	Super::OnActorHit.AddDynamic(this, &APushableBlock::OnActorHit);

	FOnTimelineFloat PushingUpdateEvent;
	PushingUpdateEvent.BindDynamic(this, &APushableBlock::MovingUpdate);
	MovingTimeline.AddInterpFloat(PushingCurve, PushingUpdateEvent);

	FOnTimelineEvent PushingFinishEvent;
	PushingFinishEvent.BindDynamic(this, &APushableBlock::MovingFinished);
	MovingTimeline.SetTimelineFinishedFunc(PushingFinishEvent);

#if WITH_EDITOR
	if (!CubeMesh)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("Unassigned Pushable Block Mesh, It will be unmoveable! Define one in BeginPlay!!"));
	}
#endif

	URequiemGameInstance::Execute_RequestLoad(URequiemGameInstance::GetInstance(GetWorld()), this);
}

void APushableBlock::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovingTimeline.TickTimeline(DeltaTime);
}

void APushableBlock::OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsPushingEnabled)
	{
		return;
	}

	const FVector PusherForwardVector = OtherActor->GetActorForwardVector();
	const FVector RoundedPushingDirection = FVector(
		FMath::RoundToDouble(PusherForwardVector.X),
		FMath::RoundToDouble(PusherForwardVector.Y),
		0
	);

	if (
		const double PushAngle = FVector::DotProduct(Hit.Normal, PusherForwardVector);
		RoundedPushingDirection == FVector::ZeroVector ||
		FMath::Abs(RoundedPushingDirection.X) + FMath::Abs(RoundedPushingDirection.Y) > 1.0f ||
		PushAngle < .95f
	)
	{
		return;
	}

	ACharacter* PushingCharacter = Cast<ACharacter>(OtherActor);

	if (
		!PushingCharacter ||
		!PushingCharacter->IsPlayerControlled() ||
		!CanPush(PushingCharacter, RoundedPushingDirection))
	{
		return;
	}

	Push(PushingCharacter, RoundedPushingDirection);
}

void APushableBlock::SaveData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	SaveGameInstance->PushableBlocks.Add(
		ID,
		FPushableBlockSaveData{IsBeingPushed() ? PushingFrom : GetActorLocation()}
	);
}

void APushableBlock::LoadData_Implementation(URequiemSaveGame* SaveGameInstance)
{
	if (const FPushableBlockSaveData* SaveData = SaveGameInstance->PushableBlocks.Find(ID))
	{
		SetActorLocation(SaveData->Position);
	}
}

bool APushableBlock::CanPush(const ACharacter* Who, const FVector& Direction) const
{
	if (
		const UCharacterMovementComponent* PusherMovementComponent = Who->GetCharacterMovement();
		!bIsPushingEnabled ||
		!CubeMesh ||
		PusherMovementComponent->MovementMode == MOVE_None ||
		!PusherMovementComponent->IsMovingOnGround() ||
		PusherMovementComponent->CurrentFloor.HitResult.GetActor() == this ||
		IsBeingPushed()
	)
	{
		return false;
	}

	const FVector MeshCenter = CubeMesh->GetComponentLocation();
	const FVector End = Direction * PushLength + MeshCenter;
	const FQuat MeshRotation = CubeMesh->GetComponentRotation().Quaternion();

	const auto BoxExtent =
		CubeMesh->GetStaticMesh()->GetBounds().BoxExtent *
		CubeMesh->GetRelativeScale3D();

	const UE::Math::TVector<double> SweepExtent =
		BoxExtent -
		FVector(0.0001, 0.0001, 0.0001);

	const FCollisionShape BoxCollision = FCollisionShape::MakeBox(SweepExtent);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;
	GetWorld()->SweepSingleByChannel(
		HitResult,
		MeshCenter,
		End,
		MeshRotation,
		ECC_WorldStatic,
		BoxCollision,
		QueryParams
	);

#if WITH_EDITOR
	float BoxLength = (BoxExtent.X == 0 ? BoxExtent.X : BoxExtent.Y) * 2;

	FVector CurrentDebugDrawPosition = MeshCenter;
	const int Boxes = FVector::DistSquared(CurrentDebugDrawPosition, End) / (BoxLength * BoxLength);
	for (int i = 1; i < Boxes; ++i)
	{
		DrawDebugBox(
			GetWorld(),
			CurrentDebugDrawPosition,
			SweepExtent,
			MeshRotation,
			FColor::Red,
			false,
			MovingTimeline.GetScaledTimelineLength()
		);

		CurrentDebugDrawPosition += Direction * BoxLength;
	}
#endif

	return !HitResult.bBlockingHit;
}

void APushableBlock::Push(ACharacter* Who, const FVector& Direction)
{
	Who->GetCharacterMovement()->DisableMovement();
	Who->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	Pusher = Who;
	PushingDirection = Direction;
	PushingFrom = GetActorLocation();
	MovingTimeline.PlayFromStart();
}
