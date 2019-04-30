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

	Blackboard->SetValueAsBool(PathIsLooping, BlackKnightPatrolPath->IsLooping); //Looping 여부 초기화
	Blackboard->SetValueAsInt(PatrolPathIndex, 0); //인덱스를 0으로 초기화
	Blackboard->SetValueAsFloat(WaitTime, 1.0f); //Wait Time 초기화
	Blackboard->SetValueAsInt(Direction, 1); //1 : Increase 방향으로 초기화


}

void ABlackKnightAIController::SetEnemy(APawn* InPawn)
{
	if (Blackboard) //블랙보드가 존재한다면
	{
		if (InPawn != nullptr)
		{
			Blackboard->SetValueAsBool(CanSeePlayer, true);
			//블랙보드 컴포넌트의 CanSeePlayer를 true로 설정
		}
		else
		{
			Blackboard->SetValueAsBool(CanSeePlayer, false);
			//매개변수로 nullptr가 들어오면 false로 설정
		}
	}
}

//블랙 나이트 캐릭터에 있는 그로기 함수와 연결
void ABlackKnightAIController::EnterGroggy()
{
	Blackboard->SetValueAsBool(IsGroggy, true);
	BrainComponent->StopLogic(FString(TEXT("Groggy"))); //비헤이비어 트리 정지
}

void ABlackKnightAIController::AfterGroggy()
{
	BrainComponent->RestartLogic(); //비헤이비어 트리 다시 시작
}

//블랙 나이트 캐릭터에 있는 함수와 연결
void ABlackKnightAIController::SetCanUseSkill()
{
	Blackboard->SetValueAsBool(CanUseSkill, true);
}