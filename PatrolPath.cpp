// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolPath.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	IsLooping = true;

	WaitTime = 1.0f;

	PathPoints.Add(FVector(-1390.0f, -5276.0f, 696.0f)); //0
	PathPoints.Add(FVector(-2065.0f, -4332.0f, 696.0f)); //1
	PathPoints.Add(FVector(-1390.0f, -3750.0f, 696.0f)); //2
	PathPoints.Add(FVector(-775.0f, -3171.0f, 696.0f)); //3
	PathPoints.Add(FVector(-1390.0f, -2525.0f, 696.0f)); //4
	PathPoints.Add(FVector(-2065.0f, -3171.0f, 696.0f)); //5
	PathPoints.Add(FVector(-1390.0f, -3750.0f, 696.0f)); //6
	PathPoints.Add(FVector(-775.0f, -4332.0f, 696.0f)); //7

}

// Called when the game starts or when spawned
void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

