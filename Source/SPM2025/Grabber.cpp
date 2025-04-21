// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


void UGrabber::Release()
{
	if (GrabbedActor)
	{
		GrabbedActor->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedActor->SetSimulatePhysics(true);
		GrabbedActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GrabbedActor = nullptr;
	}
}

void UGrabber::Grab(FHitResult HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbed Actor"));
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	HitComponent->SetSimulatePhysics(false);
	HitComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
