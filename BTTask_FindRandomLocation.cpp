// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "BlackKnightAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("FindRandomLocation");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	ABlackKnightAIController* BlackKnightController = Cast<ABlackKnightAIController>(OwnerComp.GetAIOwner());
	if (NavSys && BlackKnightController && BlackKnightController->GetPawn())
	{
		const float SearchRadius = 300.0f;

		FNavLocation RandomLocation;

		const bool bFound = NavSys->GetRandomPointInNavigableRadius(BlackKnightController->GetPawn()->GetActorLocation(), SearchRadius, RandomLocation);
		//랜덤 포인트를 찾는다

		if (bFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ABlackKnightAIController::TargetLocation, RandomLocation.Location);
			return EBTNodeResult::Succeeded;
			//랜덤 목표 지점을 찾은 후 Task실행을 위해 Succeeded 리턴
		}
	}
	return EBTNodeResult::Failed;
	//위 조건들을 만족하지 못한 경우 Failed 리턴

}

