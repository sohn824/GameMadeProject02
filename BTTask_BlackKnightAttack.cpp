// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BlackKnightAttack.h"
#include "BlackKnightCharacter.h"
#include "BlackKnightAIController.h"
#include "GameMadeProjectCharacter.h"

UBTTask_BlackKnightAttack::UBTTask_BlackKnightAttack()
{
	NodeName = TEXT("NormalAttack");
}

EBTNodeResult::Type UBTTask_BlackKnightAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ABlackKnightCharacter* BlackKnight = Cast<ABlackKnightCharacter>(ControllingPawn);
	AGameMadeProjectCharacter* PlayerCharacter = Cast<AGameMadeProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (BlackKnight && BlackKnight->BlackKnightWeapon)
	{
		BlackKnight->BlackKnightAttack(PlayerCharacter);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
	
}