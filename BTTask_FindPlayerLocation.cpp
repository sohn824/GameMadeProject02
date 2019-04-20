// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPlayerLocation.h"
#include "NavigationSystem.h"
#include "BlackKnightAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMadeProjectCharacter.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation()
{
	NodeName = TEXT("FindPlayerLocation");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	ABlackKnightAIController* BlackKnightController = Cast<ABlackKnightAIController>(OwnerComp.GetAIOwner()); //AI 컨트롤러

	AGameMadeProjectCharacter* PlayerCharacter = Cast<AGameMadeProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//플레이어 캐릭터

	if (NavSys && PlayerCharacter)
	{
		const float SearchRadius = 100.0f;

		FNavLocation PlayerLocation;

		const bool bFound = NavSys->GetRandomPointInNavigableRadius(PlayerCharacter->GetActorLocation(), SearchRadius, PlayerLocation);
		//플레이어 위치 100센티미터 이내의 포인트를 찾는다
		if (bFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ABlackKnightAIController::TargetLocation, PlayerLocation.Location);
			return EBTNodeResult::Succeeded;
			//플레이어 위치를 찾아 TargetLocation 설정 후 Task실행을 위해 Succeeded 리턴
		}
	}
	return EBTNodeResult::Failed;
	//위 조건들을 만족하지 못한 경우 Failed 리턴
}
