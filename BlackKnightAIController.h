// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "PatrolPath.h"
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
	void EnterGroggy();
	void AfterGroggy();
	void SetCanUseSkill();

	//블랙보드 키
	static const FName TargetLocation;
	static const FName CanSeePlayer;
	static const FName PatrolPathVector;
	static const FName PatrolPathIndex;
	static const FName PathIsLooping;
	static const FName Direction;
	static const FName WaitTime;
	static const FName IsInRange;
	static const FName IsGroggy;
	static const FName CanUseSkill;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Patrol")
	APatrolPath* BlackKnightPatrolPath;
};
