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

	if (Zombie && Zombie->ZombieBT) //����� ���� �����̺�� Ʈ���� �����Ѵٸ�
	{
		if (Zombie->ZombieBT->BlackboardAsset)
		{
			BBComponent->InitializeBlackboard(*Zombie->ZombieBT->BlackboardAsset); //������ ���� ����
		}
		BTComponent->StartTree(*(Zombie->ZombieBT)); //���� �����̺�� Ʈ���� ����
	}

}