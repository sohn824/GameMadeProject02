// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "BlackKnightCharacter.generated.h"

UCLASS()
class GAMEMADEPROJECT_API ABlackKnightCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlackKnightCharacter();

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	float BlackKnightHealth;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	class UPawnSensingComponent* PawnSensingComponent;
	//다른 폰을 감지할 수 있는 PawnSensingComponent

	float LastSeenTime; //마지막으로 플레이어를 발견한 시간
	bool bSensedTarget; //플레이어를 감지했는지 여부


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() //멤버 함수
	void OnSeePlayer(APawn* Pawn); //PawnSensingComponent의 OnSeePawn()에 연결해줄 함수

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
