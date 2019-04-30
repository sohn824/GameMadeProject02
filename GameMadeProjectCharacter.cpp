// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GameMadeProjectCharacter.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "MyAnimInstance.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"

//////////////////////////////////////////////////////////////////////////
// AGameMadeProjectCharacter

AGameMadeProjectCharacter::AGameMadeProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Initialize Character Attributes
	CurrentHealth = 200.0f;
	MaxHealth = 200.0f;
	CurrentMana = 200.0f;
	MaxMana = 200.0f;
	UpgradedStrength = 1.0f;

	// Initialize Skill Variables
	IsHealing = false;
	IsSprinting = false;
	IsDefending = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		HEALING_EFFECT(TEXT("/Game/InfinityBladeEffects/Effects/FX_Cines/Ausar/P_Ausar_Resurrection.P_Ausar_Resurrection"));

	if (HEALING_EFFECT.Succeeded())
	{
		HealingEffect = HEALING_EFFECT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		HEALTHPOTION_EFFECT(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_HealthPotion.P_HealthPotion"));

	if (HEALTHPOTION_EFFECT.Succeeded())
	{
		HealthPotionEffect = HEALTHPOTION_EFFECT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		MANAPOTION_EFFECT(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_ManaPotion.P_ManaPotion"));

	if (MANAPOTION_EFFECT.Succeeded())
	{
		ManaPotionEffect = MANAPOTION_EFFECT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DOUBLESLASH_MONTAGE(TEXT("/Game/GameMadeProject/Characters/Player/Animations/Skill01Montage.Skill01Montage"));

	if (DOUBLESLASH_MONTAGE.Succeeded())
	{
		DoubleSlashMontage = DOUBLESLASH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		JUMPSLASH_MONTAGE(TEXT("/Game/GameMadeProject/Characters/Player/Animations/Skill02Montage.Skill02Montage"));

	if (JUMPSLASH_MONTAGE.Succeeded())
	{
		JumpSlashMontage = JUMPSLASH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WHIRLWIND_MONTAGE(TEXT("/Game/GameMadeProject/Characters/Player/Animations/Skill03Montage.Skill03Montage"));

	if (WHIRLWIND_MONTAGE.Succeeded())
	{
		WhirlwindMontage = WHIRLWIND_MONTAGE.Object;
	}
	// Initialize Attack Variables
	IsWeaponEquiped = false;
	IsAttacking = false;
	SaveAttack = false;
	ComboCounter = 0;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE1(TEXT("/Game/GameMadeProject/Characters/Player/Animations/Combo_1_Montage.Combo_1_Montage"));

	if (ATTACK_MONTAGE1.Succeeded())
	{
		AttackMontage1 = ATTACK_MONTAGE1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE2(TEXT("/Game/GameMadeProject/Characters/Player/Animations/Combo_2_Montage.Combo_2_Montage"));

	if (ATTACK_MONTAGE2.Succeeded())
	{
		AttackMontage2 = ATTACK_MONTAGE2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE3(TEXT("/Game/GameMadeProject/Characters/Player/Animations/Combo_3_Montage.Combo_3_Montage"));

	if (ATTACK_MONTAGE3.Succeeded())
	{
		AttackMontage3 = ATTACK_MONTAGE3.Object;
	}


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


// Input

void AGameMadeProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGameMadeProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameMadeProjectCharacter::MoveRight);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGameMadeProjectCharacter::Attack);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGameMadeProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGameMadeProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGameMadeProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGameMadeProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGameMadeProjectCharacter::OnResetVR);
}


void AGameMadeProjectCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGameMadeProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AGameMadeProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AGameMadeProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGameMadeProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGameMadeProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGameMadeProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//Bind Action
void AGameMadeProjectCharacter::Attack()
{
	if (IsWeaponEquiped == true)
	{
		if (IsDefending == false)
		{
			if (IsAttacking == true)
			{
				SaveAttack = true;
			}
			else
			{
				IsAttacking = true;
				FTimerHandle TimerHandle_ResetCombo;
				if (ComboCounter == 0)
				{
					AnimDuration = PlayAnimMontage(AttackMontage1, 1.2f); //Attack Montage 1을 실행하고 실행 시간을 변수에 저장
					ComboCounter = 1;
					GetWorldTimerManager().SetTimer(TimerHandle_ResetCombo, this, &AGameMadeProjectCharacter::ResetCombo, AnimDuration, false);
					//애님 몽타주가 전부 실행되고 나면 ResetCombo 함수를 실행시킨다 (그동안 공격 입력이 들어오지 않았으므로)
				}
				else if (ComboCounter == 1)
				{
					AnimDuration = PlayAnimMontage(AttackMontage2, 1.2f); //Attack Montage 1을 실행하고 실행 시간을 변수에 저장
					ComboCounter = 2;
					GetWorldTimerManager().SetTimer(TimerHandle_ResetCombo, this, &AGameMadeProjectCharacter::ResetCombo, AnimDuration, false);
					//애님 몽타주가 전부 실행되고 나면 ResetCombo 함수를 실행시킨다 (그동안 공격 입력이 들어오지 않았으므로)			
				}
				else if (ComboCounter == 2)
				{
					AnimDuration = PlayAnimMontage(AttackMontage3, 0.8f); //Attack Montage 1을 실행하고 실행 시간을 변수에 저장
					ComboCounter = 0;
					GetWorldTimerManager().SetTimer(TimerHandle_ResetCombo, this, &AGameMadeProjectCharacter::ResetCombo, AnimDuration, false);
					//애님 몽타주가 전부 실행되고 나면 ResetCombo 함수를 실행시킨다 (그동안 공격 입력이 들어오지 않았으므로)				
				}
			}
		}
	}
}

//ResetCombo 애님 노티파이 연결 함수
void AGameMadeProjectCharacter::ResetCombo()
{
	//1이면 몽타주 실행중, 0이면 실행중 아님
	float IsAction1 = GetMesh()->AnimScriptInstance->Montage_GetPlayRate(AttackMontage1);
	float IsAction2 = GetMesh()->AnimScriptInstance->Montage_GetPlayRate(AttackMontage2);
	float IsAction3 = GetMesh()->AnimScriptInstance->Montage_GetPlayRate(AttackMontage3);

	if (IsAction1 == 0 && IsAction2 == 0 && IsAction3 == 0)
	{
		ComboCounter = 0;
		SaveAttack = false;
		IsAttacking = false;
	}
}

//SaveCombo 애님 노티파이 연결 함수
void AGameMadeProjectCharacter::SaveComboAttack()
{
	if (SaveAttack == true)
	{
		SaveAttack = false;
		FTimerHandle TimerHandle_ResetCombo;
		if (ComboCounter == 0)
		{
			AnimDuration = PlayAnimMontage(AttackMontage1, 1.2f); //Attack Montage 1을 실행하고 실행 시간을 변수에 저장
			ComboCounter = 1;
			GetWorldTimerManager().SetTimer(TimerHandle_ResetCombo, this, &AGameMadeProjectCharacter::ResetCombo, AnimDuration, false);
			//애님 몽타주가 전부 실행되고 나면 ResetCombo 함수를 실행시킨다 (그동안 공격 입력이 들어오지 않았으므로)
		}
		else if (ComboCounter == 1)
		{
			AnimDuration = PlayAnimMontage(AttackMontage2, 1.2f); //Attack Montage 1을 실행하고 실행 시간을 변수에 저장
			ComboCounter = 2;
			GetWorldTimerManager().SetTimer(TimerHandle_ResetCombo, this, &AGameMadeProjectCharacter::ResetCombo, AnimDuration, false);
			//애님 몽타주가 전부 실행되고 나면 ResetCombo 함수를 실행시킨다 (그동안 공격 입력이 들어오지 않았으므로)			
		}
		else if (ComboCounter == 2)
		{
			AnimDuration = PlayAnimMontage(AttackMontage3, 0.8f); //Attack Montage 1을 실행하고 실행 시간을 변수에 저장
			ComboCounter = 0;
			GetWorldTimerManager().SetTimer(TimerHandle_ResetCombo, this, &AGameMadeProjectCharacter::ResetCombo, AnimDuration, false);
			//애님 몽타주가 전부 실행되고 나면 ResetCombo 함수를 실행시킨다 (그동안 공격 입력이 들어오지 않았으므로)				
		}
	}
}

//Heal Skill Pressed
void AGameMadeProjectCharacter::UseHealSkill()
{
	if (IsHealing == false)
	{
		if (CurrentMana >= 10.0f)
		{
			FTimerHandle TimerHandle_ResetHeal;
			GetCharacterMovement()->DisableMovement();
			GetCharacterMovement()->StopMovementImmediately();
			CurrentMana -= 10.0f;
			CurrentHealth += 30.0f;
			if (CurrentHealth > MaxHealth)
			{
				CurrentHealth = MaxHealth;
			}
			IsHealing = true;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealingEffect, GetMesh()->GetSocketLocation("EffectSocket"), FRotator::ZeroRotator, true);
			//EffectSocket 위치에서 힐링 이펙트 발생
			GetWorldTimerManager().SetTimer(TimerHandle_ResetHeal, this, &AGameMadeProjectCharacter::ResetHeal, 1.7f, false);
			//Delay = 1.7초후 ResetHeal() 발동
		}
		else
		{
			FStringClassReference NoManaRef(TEXT("/Game/GameMadeProject/UI/UI_NoManaPopup.UI_NoManaPopup_C"));
			if (UClass* NoManaClass = NoManaRef.TryLoadClass<UUserWidget>())
			{
				UUserWidget* NoManaPopup = CreateWidget<UUserWidget>(GetWorld(), NoManaClass);
				NoManaPopup->AddToViewport();
			}
		}
	}
}
//Heal Reset Timer binding
void AGameMadeProjectCharacter::ResetHeal()
{
	IsHealing = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
//Sprint Skill Pressed
void AGameMadeProjectCharacter::UseSprintSkill()
{
	if (IsSprinting == false)
	{
		if (CurrentMana >= 15.0f)
		{
			FTimerHandle TimerHandle_ResetSprint;
			CurrentMana -= 15.0f;
			IsSprinting = true;
			GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
			GetWorldTimerManager().SetTimer(TimerHandle_ResetSprint, this, &AGameMadeProjectCharacter::ResetSprint, 2.0f, false);
			//Delay = 2.0초 후 ResetSprint() 발동
		}
		else
		{
			FStringClassReference NoManaRef(TEXT("/Game/GameMadeProject/UI/UI_NoManaPopup.UI_NoManaPopup_C"));
			if (UClass* NoManaClass = NoManaRef.TryLoadClass<UUserWidget>())
			{
				UUserWidget* NoManaPopup = CreateWidget<UUserWidget>(GetWorld(), NoManaClass);
				NoManaPopup->AddToViewport();
			}
		}
	}
}
//Sprint Reset Timer binding
void AGameMadeProjectCharacter::ResetSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	IsSprinting = false;
}

void AGameMadeProjectCharacter::UseDoubleSlash()
{
	if (CurrentMana >= 10.0f)
	{
		FTimerHandle TimerHandle_ResetDoubleSlash;
		CurrentMana -= 10.0f;
		IsAttacking = true;
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();
		UpgradedStrength *= 2;
		PlayAnimMontage(DoubleSlashMontage, 1.5f);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetDoubleSlash, this, &AGameMadeProjectCharacter::ResetDoubleSlash, 1.7f, false);
		//애님 몽타주가 종료되면 리셋함수 실행 
	}
	else
	{
		FStringClassReference NoManaRef(TEXT("/Game/GameMadeProject/UI/UI_NoManaPopup.UI_NoManaPopup_C"));
		if (UClass* NoManaClass = NoManaRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* NoManaPopup = CreateWidget<UUserWidget>(GetWorld(), NoManaClass);
			NoManaPopup->AddToViewport();
		}
	}
}

void AGameMadeProjectCharacter::ResetDoubleSlash()
{
	UpgradedStrength /= 2;
	IsAttacking = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AGameMadeProjectCharacter::UseJumpSlash()
{
	if (CurrentMana >= 15.0f)
	{
		FTimerHandle TimerHandle_ResetJumpSlash;
		CurrentMana -= 15.0f;
		IsAttacking = true;
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();
		UpgradedStrength *= 3;
		PlayAnimMontage(JumpSlashMontage, 1.9f);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetJumpSlash, this, &AGameMadeProjectCharacter::ResetJumpSlash, 1.9f, false);
		//애님 몽타주가 종료되면 리셋함수 실행 
	}
	else
	{
		FStringClassReference NoManaRef(TEXT("/Game/GameMadeProject/UI/UI_NoManaPopup.UI_NoManaPopup_C"));
		if (UClass* NoManaClass = NoManaRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* NoManaPopup = CreateWidget<UUserWidget>(GetWorld(), NoManaClass);
			NoManaPopup->AddToViewport();
		}
	}
}

void AGameMadeProjectCharacter::ResetJumpSlash()
{
	UpgradedStrength /= 3;
	IsAttacking = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AGameMadeProjectCharacter::UseWhirlwind()
{
	if (CurrentMana >= 20.0f)
	{
		FTimerHandle TimerHandle_ResetWhirlwind;
		CurrentMana -= 20.0f;
		IsAttacking = true;
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();
		UpgradedStrength *= 2;
		PlayAnimMontage(WhirlwindMontage, 2.0f);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetWhirlwind, this, &AGameMadeProjectCharacter::ResetWhirlwind, 2.4f, false);
		//애님 몽타주가 종료되면 리셋함수 실행
	}
	else
	{
		FStringClassReference NoManaRef(TEXT("/Game/GameMadeProject/UI/UI_NoManaPopup.UI_NoManaPopup_C"));
		if (UClass* NoManaClass = NoManaRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* NoManaPopup = CreateWidget<UUserWidget>(GetWorld(), NoManaClass);
			NoManaPopup->AddToViewport();
		}
	}
}

void AGameMadeProjectCharacter::ResetWhirlwind()
{
	UpgradedStrength /= 2;
	IsAttacking = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AGameMadeProjectCharacter::UseHealPotion()
{
	CurrentHealth += 50;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealthPotionEffect, GetMesh()->GetSocketLocation("EffectSocket"), FRotator::ZeroRotator, true);
	//EffectSocket 위치에서 힐 포션 이펙트 발생
}

void AGameMadeProjectCharacter::UseManaPotion()
{
	CurrentMana += 50;
	if (CurrentMana > MaxMana)
	{
		CurrentMana = MaxMana;
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ManaPotionEffect, GetMesh()->GetSocketLocation("EffectSocket"), FRotator::ZeroRotator, true);
	//EffectSocket 위치에서 마나 포션 이펙트 발생
}