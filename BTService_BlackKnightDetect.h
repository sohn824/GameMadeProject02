// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_BlackKnightDetect.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMADEPROJECT_API UBTService_BlackKnightDetect : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_BlackKnightDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
