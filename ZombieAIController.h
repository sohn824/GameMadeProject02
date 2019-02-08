// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMADEPROJECT_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

	virtual void Possess(class APawn* InPawn) override;
};
