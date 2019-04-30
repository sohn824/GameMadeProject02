// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_BlackKnightDetect.h"
#include "BlackKnightAIController.h"
#include "BlackKnightCharacter.h"
#include "GameMadeProjectCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_BlackKnightDetect::UBTService_BlackKnightDetect()
{
	NodeName = TEXT("BlackKnightDetect");
	Interval = 1.0f;
}

void UBTService_BlackKnightDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); //���� ai ��Ʈ�ѷ��� ��Ʈ������ �� ������Ʈ

	AGameMadeProjectCharacter* PlayerCharacter = Cast<AGameMadeProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	float distance = FVector::Dist(PlayerCharacter->GetActorLocation(), ControllingPawn->GetActorLocation());
	if (distance <= 350.0f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABlackKnightAIController::IsInRange, true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABlackKnightAIController::IsInRange, false);
	}
}