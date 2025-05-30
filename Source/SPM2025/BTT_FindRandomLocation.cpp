// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_FindRandomLocation::UBTT_FindRandomLocation(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ObjectInitializer}
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
				auto* const Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);

				const float minDist = 300.f;
				const int maxTries = 5;

				for (int i = 0; i < maxTries; ++i)
				{
					
					if (NavSys->GetRandomPointInNavigableRadius(Orgin, SearchRadius, NavLoc))
					{
						if (Player && FVector::Dist(NavLoc.Location, Player->GetActorLocation()) < minDist)
						{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLoc.Location);
						//Avsluta med success
						FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
						}
					}
				}
				
			}
		}
	}
	//Avsluta med failed
	return EBTNodeResult::Failed;
}
