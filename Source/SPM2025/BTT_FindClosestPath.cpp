// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindClosestPath.h"

#include "NPC.h"
#include "NPC_AIController.h"
#include "PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_FindClosestPath::UBTT_FindClosestPath()
{
	NodeName = "Find Closest Path";
}

EBTNodeResult::Type UBTT_FindClosestPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const cont = OwnerComp.GetAIOwner()->GetPawn();

	if (!cont)
	{
		return EBTNodeResult::Failed;
	}

	const FVector AIPosition = cont->GetActorLocation();
	TArray<AActor*> PathFound;
	UGameplayStatics::GetAllActorsOfClass(cont->GetWorld(),APatrolPath::StaticClass(),PathFound);

	float ClosestDistance = FLT_MAX;
	int ClosestIndex = -1;
	APatrolPath* ClosestPath = nullptr;

	for (auto* Actor : PathFound)
	{
		APatrolPath* Path = Cast<APatrolPath>(Actor);
		if (!Path) continue;

		for (int i = 0; i < Path->Num(); ++i)
		{
			float Dist = FVector::Dist(AIPosition,Path->GetPatrolPoint(i));
			if (Dist < ClosestDistance)
			{
				ClosestDistance = Dist;
				ClosestIndex = i;
				ClosestPath = Path;
			}
		}
	}
	if (ClosestPath && ClosestIndex != -1)
	{
		auto* BC = OwnerComp.GetBlackboardComponent();
		BC->SetValueAsObject(ClosestPathKey.SelectedKeyName, ClosestPath);
		BC->SetValueAsInt(ClosestIndexKey.SelectedKeyName, ClosestIndex);
		ANPC* const NPC = Cast<ANPC>(cont);
		if (NPC)
		{
			
			NPC->SetPatrolPath(ClosestPath); // Create this setter if needed
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
