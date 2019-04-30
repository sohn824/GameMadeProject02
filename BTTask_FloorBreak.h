// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FloorBreak.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMADEPROJECT_API UBTTask_FloorBreak : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FloorBreak();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
