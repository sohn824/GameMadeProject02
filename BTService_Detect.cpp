// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "ZombieAIController.h"
#include "ZombieCharacter.h"
#include "GameMadeProjectCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); //현재 ai 컨트롤러가 컨트롤중인 폰 오브젝트
	
	AGameMadeProjectCharacter* PlayerCharacter = Cast<AGameMadeProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	float distance = FVector::Dist(PlayerCharacter->GetActorLocation(), ControllingPawn->GetActorLocation());
	if (distance <= 250.0f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AZombieAIController::IsInRange , true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AZombieAIController::IsInRange , false);
	}
}
