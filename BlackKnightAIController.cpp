// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackKnightAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BlackKnightCharacter.h"

const FName ABlackKnightAIController::TargetLocation(TEXT("TargetLocation"));
const FName ABlackKnightAIController::CanSeePlayer(TEXT("CanSeePlayer"));
const FName ABlackKnightAIController::PatrolPathVector(TEXT("PatrolPathVector"));
const FName ABlackKnightAIController::PatrolPathIndex(TEXT("PatrolPathIndex"));
const FName ABlackKnightAIController::PathIsLooping(TEXT("PathIsLooping"));
const FName ABlackKnightAIController::Direction(TEXT("Direction"));
const FName ABlackKnightAIController::WaitTime(TEXT("WaitTime"));
const FName ABlackKnightAIController::IsInRange(TEXT("IsInRange"));
const FName ABlackKnightAIController::IsGroggy(TEXT("IsGroggy"));
const FName ABlackKnightAIController::CanUseSkill(TEXT("CanUseSkill"));

ABlackKnightAIController::ABlackKnightAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BBObject(TEXT("/Game/GameMadeProject/AI/BB_BlackKnight.BB_BlackKnight"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTObject(TEXT("/Game/GameMadeProject/AI/BT_BlackKnight.BT_BlackKnight"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	BlackKnightPatrolPath = NewObject<APatrolPath>();

}

void ABlackKnightAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		RunBehaviorTree(BTAsset);
	}

	Blackboard->SetValueAsBool(PathIsLooping, BlackKnightPatrolPath->IsLooping); //Looping ���� �ʱ�ȭ
	Blackboard->SetValueAsInt(PatrolPathIndex, 0); //�ε����� 0���� �ʱ�ȭ
	Blackboard->SetValueAsFloat(WaitTime, 1.0f); //Wait Time �ʱ�ȭ
	Blackboard->SetValueAsInt(Direction, 1); //1 : Increase �������� �ʱ�ȭ


}

void ABlackKnightAIController::SetEnemy(APawn* InPawn)
{
	if (Blackboard) //�����尡 �����Ѵٸ�
	{
		if (InPawn != nullptr)
		{
			Blackboard->SetValueAsBool(CanSeePlayer, true);
			//������ ������Ʈ�� CanSeePlayer�� true�� ����
		}
		else
		{
			Blackboard->SetValueAsBool(CanSeePlayer, false);
			//�Ű������� nullptr�� ������ false�� ����
		}
	}
}

//�� ����Ʈ ĳ���Ϳ� �ִ� �׷α� �Լ��� ����
void ABlackKnightAIController::EnterGroggy()
{
	Blackboard->SetValueAsBool(IsGroggy, true);
	BrainComponent->StopLogic(FString(TEXT("Groggy"))); //�����̺�� Ʈ�� ����
}

void ABlackKnightAIController::AfterGroggy()
{
	BrainComponent->RestartLogic(); //�����̺�� Ʈ�� �ٽ� ����
}

//�� ����Ʈ ĳ���Ϳ� �ִ� �Լ��� ����
void ABlackKnightAIController::SetCanUseSkill()
{
	Blackboard->SetValueAsBool(CanUseSkill, true);
}