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
}

void ABlackKnightAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		RunBehaviorTree(BTAsset);
	}
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

