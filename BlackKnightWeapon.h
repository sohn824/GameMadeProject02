// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackKnightWeapon.generated.h"

UCLASS()
class GAMEMADEPROJECT_API ABlackKnightWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackKnightWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* Weapon;



};
