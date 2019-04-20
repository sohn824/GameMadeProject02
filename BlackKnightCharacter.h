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
	//�ٸ� ���� ������ �� �ִ� PawnSensingComponent

	float LastSeenTime; //���������� �÷��̾ �߰��� �ð�
	bool bSensedTarget; //�÷��̾ �����ߴ��� ����


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() //��� �Լ�
	void OnSeePlayer(APawn* Pawn); //PawnSensingComponent�� OnSeePawn()�� �������� �Լ�

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
