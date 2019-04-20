// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BlackboardData.h"
#include "ZombieCharacter.h"

const FName AZombieAIController::TargetEnemy(TEXT("TargetEnemy"));
const FName AZombieAIController::IsInRange(TEXT("IsInRange"));

AZombieAIController::AZombieAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));

}

void AZombieAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AZombieCharacter* Zombie = Cast<AZombieCharacter>(InPawn);

	if (Zombie && Zombie->ZombieBT) //좀비와 좀비 비헤이비어 트리가 존재한다면
	{
		if (Zombie->ZombieBT->BlackboardAsset)
		{
			BBComponent->InitializeBlackboard(*Zombie->ZombieBT->BlackboardAsset); //블랙보드 에셋 지정
		}
		BTComponent->StartTree(*(Zombie->ZombieBT)); //좀비 비헤이비어 트리를 시작
	}
}

void AZombieAIController::SetEnemy(APawn* InPawn)
{
	if (BBComponent) //블랙보드 컴포넌트가 존재한다면
	{
		//BBComponent->SetValue<UBlackboardKeyType_Object>("TargetEnemy", InPawn);
		BBComponent->SetValueAsObject(TargetEnemy, InPawn);
		//블랙보드 컴포넌트의 TargetEnemy 오브젝트형 변수를 매개변수인 InPawn(플레이어가 들어올 것)으로 설정해준다
	}
}