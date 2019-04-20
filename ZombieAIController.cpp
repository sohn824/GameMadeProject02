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

	if (Zombie && Zombie->ZombieBT) //����� ���� �����̺�� Ʈ���� �����Ѵٸ�
	{
		if (Zombie->ZombieBT->BlackboardAsset)
		{
			BBComponent->InitializeBlackboard(*Zombie->ZombieBT->BlackboardAsset); //������ ���� ����
		}
		BTComponent->StartTree(*(Zombie->ZombieBT)); //���� �����̺�� Ʈ���� ����
	}
}

void AZombieAIController::SetEnemy(APawn* InPawn)
{
	if (BBComponent) //������ ������Ʈ�� �����Ѵٸ�
	{
		//BBComponent->SetValue<UBlackboardKeyType_Object>("TargetEnemy", InPawn);
		BBComponent->SetValueAsObject(TargetEnemy, InPawn);
		//������ ������Ʈ�� TargetEnemy ������Ʈ�� ������ �Ű������� InPawn(�÷��̾ ���� ��)���� �������ش�
	}
}