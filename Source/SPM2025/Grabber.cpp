// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GrabbedActor) return;

	FVector Location = GrabbedActor->GetComponentLocation();
	FVector Extent = GrabbedActor->Bounds.BoxExtent;

	TArray<UPrimitiveComponent*> OverlappingComponents;
	bool bIsOverlapping = UKismetSystemLibrary::BoxOverlapComponents(
		GetWorld(),
		Location,
		Extent,
		TArray<TEnumAsByte<EObjectTypeQuery>>{UEngineTypes::ConvertToObjectType(ECC_WorldStatic)},
		UPrimitiveComponent::StaticClass(),
		TArray<AActor*>{GetOwner(), GrabbedActor->GetOwner()},
		OverlappingComponents
	);
	DrawDebugBox(GetWorld(), Location, Extent, FColor::Yellow, false, 0.1f);

	bCanDrop = !bIsOverlapping;
}

void UGrabber::Release()
{
	if (GrabbedActor)
	{

		if (!bCanDrop)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't release — object is overlapping with the world!"));
			return;
		}
		
		GrabbedActor->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedActor->SetSimulatePhysics(true);
		GrabbedActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GrabbedActor->SetCollisionResponseToAllChannels(ECR_Block);
		GrabbedActor = nullptr;
	}
}

void UGrabber::ReleaseAtPos(FVector Location, FRotator Rotation)
{
	if (GrabbedActor)
	{

		if (!bCanDrop)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't release — object is overlapping with the world!"));
			return;
		}
		
		GrabbedActor->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedActor->SetSimulatePhysics(true);
		GrabbedActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GrabbedActor->SetCollisionResponseToAllChannels(ECR_Block);
		GrabbedActor->GetOwner()->SetActorLocationAndRotation(Location, Rotation);
		GrabbedActor = nullptr;
	}
}


void UGrabber::Grab(FHitResult HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbed Actor"));
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	HitComponent->SetSimulatePhysics(false);
	HitComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	AActor* Owner = GetOwner();
	UCameraComponent* Camera = Owner->FindComponentByClass<UCameraComponent>();
	FVector RelativeOffset(100.f, -50.f, -50.f);  // Forward, Left, Down from camera
	FRotator OffsetRotation = FRotator::ZeroRotator;

	HitComponent->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);
	HitComponent->SetRelativeLocation(RelativeOffset);
	HitComponent->SetRelativeRotation(OffsetRotation);
	
	UStaticMeshComponent* Actor = Cast<UStaticMeshComponent>(HitResult.GetComponent());
	GrabbedActor = Actor;

}
