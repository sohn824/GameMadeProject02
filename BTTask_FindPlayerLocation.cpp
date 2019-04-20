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

	ABlackKnightAIController* BlackKnightController = Cast<ABlackKnightAIController>(OwnerComp.GetAIOwner()); //AI ��Ʈ�ѷ�

	AGameMadeProjectCharacter* PlayerCharacter = Cast<AGameMadeProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//�÷��̾� ĳ����

	if (NavSys && PlayerCharacter)
	{
		const float SearchRadius = 100.0f;

		FNavLocation PlayerLocation;

		const bool bFound = NavSys->GetRandomPointInNavigableRadius(PlayerCharacter->GetActorLocation(), SearchRadius, PlayerLocation);
		//�÷��̾� ��ġ 100��Ƽ���� �̳��� ����Ʈ�� ã�´�
		if (bFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ABlackKnightAIController::TargetLocation, PlayerLocation.Location);
			return EBTNodeResult::Succeeded;
			//�÷��̾� ��ġ�� ã�� TargetLocation ���� �� Task������ ���� Succeeded ����
		}
	}
	return EBTNodeResult::Failed;
	//�� ���ǵ��� �������� ���� ��� Failed ����
}
