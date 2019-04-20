// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "GameMadeProjectCharacter.generated.h"

UCLASS(config=Game)
class AGameMadeProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AGameMadeProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//Character Attributes
	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	float CurrentHealth;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	float CurrentMana;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	float MaxMana;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
	int UpgradedStrength;
	

	//Healing Skill
	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	bool IsHealing;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effect", Meta = (AllowPrivateAccess = true))
	UParticleSystem* HealingEffect;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UseHealSkill();

	void ResetHeal();

	//Sprinting Skill
	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	bool IsSprinting;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UseSprintSkill();
	
	void ResetSprint();

	//Defend Skill
	UPROPERTY(BlueprintReadWrite, Category = "Skill")
	bool IsDefending;
	
	//Attack Skills
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DoubleSlashMontage;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UseDoubleSlash();

	void ResetDoubleSlash();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* JumpSlashMontage;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UseJumpSlash();

	void ResetJumpSlash();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* WhirlwindMontage;

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UseWhirlwind();

	void ResetWhirlwind();

	//Attacking Variables and Functions
	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool IsWeaponEquiped;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool IsAttacking;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool SaveAttack;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	int ComboCounter;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage3;

	float AnimDuration;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ResetCombo();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SaveComboAttack();

	//Use Potion
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseHealPotion();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseManaPotion();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effect", Meta = (AllowPrivateAccess = true))
	UParticleSystem* HealthPotionEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effect", Meta = (AllowPrivateAccess = true))
	UParticleSystem* ManaPotionEffect;


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void Attack();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


};

