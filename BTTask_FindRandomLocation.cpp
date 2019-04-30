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
		//���� ����Ʈ�� ã�´�

		if (bFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ABlackKnightAIController::TargetLocation, RandomLocation.Location);
			return EBTNodeResult::Succeeded;
			//���� ��ǥ ������ ã�� �� Task������ ���� Succeeded ����
		}
	}
	return EBTNodeResult::Failed;
	//�� ���ǵ��� �������� ���� ��� Failed ����

}

