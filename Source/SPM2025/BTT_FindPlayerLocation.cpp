// BTT_FindPlayerLocation.cpp

#include "BTT_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

UBTT_FindPlayerLocation::UBTT_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
    : UBTTask_BlackboardBase(ObjectInitializer)
{
    NodeName = TEXT("Find Player Location");
    // This task will use Blackboard keys manually inside ExecuteTask
}

EBTNodeResult::Type UBTT_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    // Read the raw sound location key (make sure this matches your blackboard!)
    FVector SoundLocation = Blackboard->GetValueAsVector("SoundLocation");

    // Sanity check for a valid vector
    if (SoundLocation.IsNearlyZero())
    {
        UE_LOG(LogTemp, Warning, TEXT("SoundLocation invalid or zero."));
        return EBTNodeResult::Failed;
    }

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys)
    {
        UE_LOG(LogTemp, Warning, TEXT("Navigation system not found"));
        return EBTNodeResult::Failed;
    }

    FNavLocation ProjectedLocation;
    if (!NavSys->ProjectPointToNavigation(SoundLocation, ProjectedLocation))
    {
        UE_LOG(LogTemp, Warning, TEXT("SoundLocation not on navmesh: %s"), *SoundLocation.ToString());
        return EBTNodeResult::Failed;
    }

    FVector FinalLocation = ProjectedLocation.Location;

    if (SearchRadius > 0.f)
    {
        FNavLocation RandomLocation;
        if (NavSys->GetRandomReachablePointInRadius(FinalLocation, SearchRadius, RandomLocation))
        {
            FinalLocation = RandomLocation.Location;
        }
    }

    // Now set the *TargetLocation* key — separate from SoundLocation!
    Blackboard->SetValueAsVector("TargetLocation", FinalLocation);

    UE_LOG(LogTemp, Warning, TEXT("Set TargetLocation to: %s"), *FinalLocation.ToString());

    // Debug sphere
    DrawDebugSphere(GetWorld(), FinalLocation, 50.f, 12, FColor::Green, false, 2.f);

    return EBTNodeResult::Succeeded;
}