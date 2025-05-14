// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindPatrolPoint.h"

#include "NPC.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FindPatrolPoint::UBTT_FindPatrolPoint(FObjectInitializer const& ObjectInitializer):
	UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = "Find Patrol Point";
}

EBTNodeResult::Type UBTT_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Hämta AI controller
	if (auto const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		//Hämta blackboardcomponent från behavior tree
		if (auto* const BC = OwnerComp.GetBlackboardComponent())
		{
			//Hämta current patrol point från index i blackboard
			auto const Index = BC->GetValueAsInt(GetSelectedBlackboardKey());

			//Hämta AI(NPC)
			if (auto* npc = Cast<ANPC>(cont->GetPawn()))
			{
				auto* PatrolPath = npc->GetPatrolPath();
				if (!PatrolPath)
				{
					UE_LOG(LogTemp, Error, TEXT("NPC '%s' has no patrol path assigned."), *npc->GetName());
					return EBTNodeResult::Failed;
				}

				if (!PatrolPath->PatrolPoints.IsValidIndex(Index))
				{
					UE_LOG(LogTemp, Error, TEXT("Invalid patrol point index %d for NPC '%s' on path '%s'"),
						   Index, *npc->GetName(), *PatrolPath->GetName());
					return EBTNodeResult::Failed;
				}
				//Hämta Vectorn för Patrol path - Sen hämta en point av path arrayn.
				auto const Point = PatrolPath->GetPatrolPoint(Index);
				
				//Convertera point till global
				auto const GlobalPoint = PatrolPath->GetActorTransform().TransformPosition(Point);
				
				BC->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				//Avsluta med success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
