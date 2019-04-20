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

	//이 비헤이비어 트리를 실행하는 좀비 캐릭터
	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ZombieCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//타겟으로 설정된 플레이어 or 팀
	AGameMadeProjectCharacter* Target = Cast<AGameMadeProjectCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombieAIController::TargetEnemy));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - ZombieCharacter->GetActorLocation(); //플레이어와 좀비 사이의 시야 벡터
	LookVector.Z = 0.0f; //Z축값은 없애줌
	FRotator TargetRotator = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ZombieCharacter->SetActorRotation(FMath::RInterpTo(ZombieCharacter->GetActorRotation(), TargetRotator, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}

