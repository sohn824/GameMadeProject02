// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FloorBreak.h"
#include "BlackKnightCharacter.h"
#include "BlackKnightAIController.h"

UBTTask_FloorBreak::UBTTask_FloorBreak()
{
	NodeName = TEXT("Floor Break");
}

EBTNodeResult::Type UBTTask_FloorBreak::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ABlackKnightCharacter* BlackKnight = Cast<ABlackKnightCharacter>(ControllingPawn);
	if (BlackKnight && BlackKnight->BlackKnightWeapon)
	{
		BlackKnight->UseFloorBreak();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
