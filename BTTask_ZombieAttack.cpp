// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ZombieAttack.h"
#include "ZombieCharacter.h"
#include "GameMadeProjectCharacter.h"

UBTTask_ZombieAttack::UBTTask_ZombieAttack()
{
	NodeName = TEXT("ZombieAttack");
}

EBTNodeResult::Type UBTTask_ZombieAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AZombieCharacter* ControllingZombie = Cast<AZombieCharacter>(ControllingPawn);
	AGameMadeProjectCharacter* PlayerCharacter = Cast<AGameMadeProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ControllingZombie->ExecuteMeleeDamage(PlayerCharacter);
	return EBTNodeResult::Succeeded;
}
