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