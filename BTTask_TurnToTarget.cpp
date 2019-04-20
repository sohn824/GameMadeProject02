// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
#include "ZombieCharacter.h"
#include "ZombieAIController.h"
#include "GameMadeProjectCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//�� �����̺�� Ʈ���� �����ϴ� ���� ĳ����
	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ZombieCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//Ÿ������ ������ �÷��̾� or ��
	AGameMadeProjectCharacter* Target = Cast<AGameMadeProjectCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombieAIController::TargetEnemy));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - ZombieCharacter->GetActorLocation(); //�÷��̾�� ���� ������ �þ� ����
	LookVector.Z = 0.0f; //Z�ప�� ������
	FRotator TargetRotator = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ZombieCharacter->SetActorRotation(FMath::RInterpTo(ZombieCharacter->GetActorRotation(), TargetRotator, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}

