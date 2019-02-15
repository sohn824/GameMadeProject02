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
	NodeName = TEXT("FindNearLocation"); //�����̺�� Ʈ���� ��Ÿ�� '�½�ũ ����� �̸�'
}

EBTNodeResult::Type UBTTask_FindNearLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	AZombieAIController* ZombieController = Cast<AZombieAIController>(OwnerComp.GetAIOwner());

	if (NavSys && ZombieController && ZombieController->GetPawn()) //���� null üũ
	{
		const float SearchRadius = 700.0f; //Ž�� ������

		FNavLocation RandomPoint;

		const bool bFound = NavSys->GetRandomPointInNavigableRadius(ZombieController->GetPawn()->GetActorLocation(), SearchRadius, RandomPoint);
		//���� ����Ʈ�� ã�´�

		if (bFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(GetSelectedBlackboardKey(), RandomPoint.Location);
			//���õ� ������ Ű���� ã�� ���� ����Ʈ�� ����

			return EBTNodeResult::Succeeded;
			//���� ��ǥ ������ ã�� �� Task������ ���� Succeeded ����
		}
	}
	return EBTNodeResult::Failed;
	//�� ���ǵ��� �������� ���� ��� Failed ����
}