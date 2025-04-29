// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GrabbedActor)
	{

		FVector CameraLocation = Camera->GetComponentLocation();
		FVector Forward = Camera->GetForwardVector();
		FVector Right = Camera->GetRightVector();
		FVector Up = Camera->GetUpVector();
		
		float ForwardDistance = 75.f;  
		float RightOffset = -50.f;       
		float UpOffset = -50.f;            
		
		FVector GrabLocation = CameraLocation 
			+ Forward * (ForwardDistance + LocationOffset.X)
			+ Right * (RightOffset + LocationOffset.Y)
			+ Up * (UpOffset + LocationOffset.Z);

		GrabbedActor->SetActorLocation(GrabLocation);
		
		FRotator CameraRot = Camera->GetComponentRotation();
		GrabbedActor->SetActorRotation(CameraRot + RotationOffset);
	}
	
}

void UGrabber::Release()
{
	if (!GrabbedActor) return;
	

	FVector Location = GrabbedActor->GetActorLocation();
	FVector Extent = FVector(50.f, 50.f, 50.f); 

	FBox ActorBounds = GrabbedActor->GetComponentsBoundingBox();
	if (ActorBounds.IsValid)
	{
		Location = ActorBounds.GetCenter();
		Extent = ActorBounds.GetExtent();
	}

	TArray<UPrimitiveComponent*> OverlappingComponents;
	bool bIsOverlapping = UKismetSystemLibrary::BoxOverlapComponents(
		GetWorld(),
		Location,
		Extent,
		TArray<TEnumAsByte<EObjectTypeQuery>>{UEngineTypes::ConvertToObjectType(ECC_WorldStatic)},
		UPrimitiveComponent::StaticClass(),
		TArray<AActor*>{GetOwner(), GrabbedActor},
		OverlappingComponents
	);

	DrawDebugBox(GetWorld(), Location, Extent, FColor::Yellow, false, 0.1f);

	if (bIsOverlapping)
	{
		return; 
	}
	LocationOffset = FVector::ZeroVector;
	RotationOffset = FRotator::ZeroRotator;
	
	GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FRotator CurrentRotation = GrabbedActor->GetActorRotation();
	GrabbedActor->SetActorRotation(FRotator(0.f, CurrentRotation.Yaw, 0.f));	
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GrabbedActor->GetComponents(PrimitiveComponents);

	for (UPrimitiveComponent* Prim : PrimitiveComponents)
	{
		if (Prim)
		{
			Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Prim->SetCollisionResponseToAllChannels(ECR_Block);
			if (GrabbedActorHadPhysics)
			{
				Prim->SetSimulatePhysics(true);
			}
		}
	}
	GrabbedActorHadPhysics = false;
	ItemDropped.Broadcast(GrabbedActor);
	GrabbedActor = nullptr;
}

void UGrabber::ReleaseAtPos(FVector Location, FRotator Rotation)
{
	if (!GrabbedActor) return;

	GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GrabbedActor->GetComponents(PrimitiveComponents);
	LocationOffset = FVector::ZeroVector;
	RotationOffset = FRotator::ZeroRotator;
	for (UPrimitiveComponent* Prim : PrimitiveComponents)
	{
		if (Prim)
		{
			Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Prim->SetCollisionResponseToAllChannels(ECR_Block);
			if (GrabbedActorHadPhysics)
			{
				Prim->SetSimulatePhysics(true);
			}
		}
	}
	GrabbedActorHadPhysics = false;

	GrabbedActor->SetActorLocationAndRotation(Location, Rotation);

	ItemDropped.Broadcast(GrabbedActor);
	GrabbedActor = nullptr;
}


void UGrabber::Grab(AActor* HitActor, FVector ExtraLocationOffset, FRotator ExtraRotationOffset)
{
	if (!HitActor) return;
	LocationOffset = ExtraLocationOffset;
	RotationOffset = ExtraRotationOffset;
	
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	HitActor->GetComponents(PrimitiveComponents);

	

	for (auto* Prim : PrimitiveComponents)
	{
		GrabbedActorHadPhysics = Prim->IsSimulatingPhysics();
		if (Prim)
		{
			Prim->SetSimulatePhysics(false);
			Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Prim->SetCollisionResponseToAllChannels(ECR_Overlap);
		}
	}
	GrabbedActor = HitActor;
	ItemGrabbed.Broadcast(GrabbedActor);
}