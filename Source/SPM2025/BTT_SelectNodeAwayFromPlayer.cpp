// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SelectNodeAwayFromPlayer.h"

#include "PlayerCharacter.h"
#include "RatAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_SelectNodeAwayFromPlayer::UBTT_SelectNodeAwayFromPlayer()
{
	NodeName = "Run Away From Player";
}
EBTNodeResult::Type UBTT_SelectNodeAwayFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    ARatAIController* RatAICon = Cast<ARatAIController>(OwnerComp.GetAIOwner());
    if (!RatAICon)
    {
        return EBTNodeResult::Failed;
    }

    APawn* AIPawn = RatAICon->GetPawn();
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    TArray<FVector> Targets = RatAICon->GetAllTargets();
    
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector CurrentLocation = AIPawn->GetActorLocation();
    
    int CurrentIndex = RatAICon->GetCurrentIndex();
    
    int EscapeIndex = GetBestEscapeLocation(Targets, CurrentLocation, PlayerLocation, CurrentIndex);

    RatAICon->SetCurrentIndex(EscapeIndex);
    RatAICon->GetBlackboardComponent()->SetValueAsVector("CurrentTarget", Targets[EscapeIndex]);
    
    return EBTNodeResult::Succeeded;
    
}

int UBTT_SelectNodeAwayFromPlayer::GetBestEscapeLocation(const TArray<FVector>& Targets, const FVector& CurrentLocation, const FVector& PlayerLocation, int32 CurrentIndex) const
{
    int NumTargets = Targets.Num();

    int NextIndex = (CurrentIndex + 1) % NumTargets;
    int PrevIndex = (CurrentIndex - 1 + NumTargets) % NumTargets; 

    FVector ToNext = (Targets[NextIndex] - CurrentLocation).GetSafeNormal();
    FVector ToPrev = (Targets[PrevIndex] - CurrentLocation).GetSafeNormal();
    FVector ToPlayer = (PlayerLocation - CurrentLocation).GetSafeNormal();

    float DotNext = FVector::DotProduct(ToNext, ToPlayer);
    float DotPrev = FVector::DotProduct(ToPrev, ToPlayer);
    
    if (DotNext < DotPrev)
    {
        return NextIndex;
    }
    return PrevIndex;
}