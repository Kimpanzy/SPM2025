// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FindRandomLocation::UBTT_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location in NavMesh";
}

EBTNodeResult::Type UBTT_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Hämta AI controller och NPC
	if (auto* const cont = Cast<ANPC_AIController>(OwnerComp.GetOwner()))
	{
		if (auto* const npc = cont->GetPawn())
		{
			//Hämta npc location och använd det som startpunkt
			auto const Orgin = npc->GetActorLocation();

			//Hämta navigation system och generera random location
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation NavLoc;
				if (NavSys->GetRandomPointInNavigableRadius(Orgin, SearchRadius, NavLoc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLoc.Location);
				}
				//Avsluta med success
				FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	//Avsluta med failed
	return EBTNodeResult::Failed;
}
