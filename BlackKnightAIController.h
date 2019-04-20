// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "AIController.h"
#include "BlackKnightAIController.generated.h"

/**
 * 
 */

UCLASS()
class GAMEMADEPROJECT_API ABlackKnightAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABlackKnightAIController();
	virtual void Possess(class APawn* InPawn) override;
	void SetEnemy(class APawn* InPawn);

	//블랙보드 키
	static const FName TargetLocation;
	static const FName CanSeePlayer;



private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
	
};
