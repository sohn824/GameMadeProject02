// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "BlackKnightWeapon.h"
#include "PatrolPath.h"
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

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	float BlackKnightMaxHealth;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	int32 BlackKnightDamage;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	bool IsGroggy;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	bool CanGroggy;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	class UPawnSensingComponent* PawnSensingComponent;
	//�ٸ� ���� ������ �� �ִ� PawnSensingComponent

	float LastSeenTime; //���������� �÷��̾ �߰��� �ð�
	bool bSensedTarget; //�÷��̾ �����ߴ��� ����

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ABlackKnightWeapon* BlackKnightWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAnimMontage* AttackAnimMontage1;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAnimMontage* AttackAnimMontage2;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAnimMontage* AttackAnimMontage3;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAnimMontage* CastingAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAnimMontage* FloorBreakAnimMontage;

	UPROPERTY(BlueprintReadWrite, Category = "AI")
	UParticleSystem* FloorBreakEffect;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAnimMontage* DeadAnimMontage;
	


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

	UFUNCTION()
	void BlackKnightAttack(AActor* HitActor);

	UFUNCTION(BlueprintCallable, Category = "AI") //�������Ʈ������ C++ ����Լ��� ȣ���� �� �ְ� ���ִ� ��ũ�� 
	void ExecuteDamage(AActor* HitActor);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void EnterGroggy();

	void AfterGroggy();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetCanUseSkill();

	void UseFloorBreak();



};
