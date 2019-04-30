// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BlackKnightAttack.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMADEPROJECT_API UBTTask_BlackKnightAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_BlackKnightAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
