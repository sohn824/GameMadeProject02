// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieAIController.h"
#include "GameMadeProjectCharacter.h"
#include "ZombieCharacter.generated.h"

UCLASS()
class GAMEMADEPROJECT_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() //멤버 함수
	void OnSeePlayer(APawn* Pawn);

	void LookingForOverlapActor(); //겹쳐진 액터를 찾을 함수

	UFUNCTION() //멤버 함수 (공격 범위 BeginOverlap 함수에 연결시킬 함수임)
	void OnAttackRangeBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	float ZombieHealth;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* ZombieBT;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	class UPawnSensingComponent* PawnSensingComponent;
	//다른 폰을 감지할 수 있는 PawnSensingComponent

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAnimMontage* AttackAnimMontage;
	//공격 애님 몽타주

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UCapsuleComponent* AttackRangeComponent;

	float LastSeenTime; //마지막으로 플레이어를 발견한 시간
	bool bSensedTarget; //플레이어를 감지했는지 여부

	UFUNCTION(BlueprintCallable, Category = "AI") //블루프린트에서도 C++ 멤버함수를 호출할 수 있게 해주는 매크로 
	void ExecuteMeleeDamage(AActor* HitActor);
	//데미지 전달 및 공격 애니메이션 실행 함수

	void EnableMovement();

};
