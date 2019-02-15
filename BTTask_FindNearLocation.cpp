// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindNearLocation.h"
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Engine.h"
#include "ZombieAIController.h"
#include "NavigationSystem.h"

UBTTask_FindNearLocation::UBTTask_FindNearLocation()
{
	NodeName = TEXT("FindNearLocation"); //비헤이비어 트리에 나타날 '태스크 노드의 이름'
}

EBTNodeResult::Type UBTTask_FindNearLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	AZombieAIController* ZombieController = Cast<AZombieAIController>(OwnerComp.GetAIOwner());

	if (NavSys && ZombieController && ZombieController->GetPawn()) //각각 null 체크
	{
		const float SearchRadius = 700.0f; //탐색 반지름

		FNavLocation RandomPoint;

		const bool bFound = NavSys->GetRandomPointInNavigableRadius(ZombieController->GetPawn()->GetActorLocation(), SearchRadius, RandomPoint);
		//랜덤 포인트를 찾는다

		if (bFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(GetSelectedBlackboardKey(), RandomPoint.Location);
			//선택된 블랙보드 키값을 찾은 랜덤 포인트로 설정

			return EBTNodeResult::Succeeded;
			//랜덤 목표 지점을 찾은 후 Task실행을 위해 Succeeded 리턴
		}
	}
	return EBTNodeResult::Failed;
	//위 조건들을 만족하지 못한 경우 Failed 리턴
}