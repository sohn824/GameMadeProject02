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

	UFUNCTION() //��� �Լ�
	void OnSeePlayer(APawn* Pawn);

	void LookingForOverlapActor(); //������ ���͸� ã�� �Լ�

	UFUNCTION() //��� �Լ� (���� ���� BeginOverlap �Լ��� �����ų �Լ���)
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
	//�ٸ� ���� ������ �� �ִ� PawnSensingComponent

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAnimMontage* AttackAnimMontage;
	//���� �ִ� ��Ÿ��

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UCapsuleComponent* AttackRangeComponent;

	float LastSeenTime; //���������� �÷��̾ �߰��� �ð�
	bool bSensedTarget; //�÷��̾ �����ߴ��� ����

	UFUNCTION(BlueprintCallable, Category = "AI") //�������Ʈ������ C++ ����Լ��� ȣ���� �� �ְ� ���ִ� ��ũ�� 
	void ExecuteMeleeDamage(AActor* HitActor);
	//������ ���� �� ���� �ִϸ��̼� ���� �Լ�

	void EnableMovement();

};
