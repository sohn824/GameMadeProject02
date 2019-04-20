// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
class UBehaviorTreeComponent;
class UBlackboardComponent;
UCLASS()
class GAMEMADEPROJECT_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
public:
	AZombieAIController();
	virtual void Possess(class APawn* InPawn) override;
	void SetEnemy(class APawn* InPawn);

	//블랙보드 키
	static const FName TargetEnemy;
	static const FName IsInRange;

private:
	UBehaviorTreeComponent* BTComponent;
	UBlackboardComponent* BBComponent;

};
