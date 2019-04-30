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
	//다른 폰을 감지할 수 있는 PawnSensingComponent

	float LastSeenTime; //마지막으로 플레이어를 발견한 시간
	bool bSensedTarget; //플레이어를 감지했는지 여부

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

	UFUNCTION() //멤버 함수
	void OnSeePlayer(APawn* Pawn); //PawnSensingComponent의 OnSeePawn()에 연결해줄 함수

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void BlackKnightAttack(AActor* HitActor);

	UFUNCTION(BlueprintCallable, Category = "AI") //블루프린트에서도 C++ 멤버함수를 호출할 수 있게 해주는 매크로 
	void ExecuteDamage(AActor* HitActor);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void EnterGroggy();

	void AfterGroggy();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetCanUseSkill();

	void UseFloorBreak();



};
