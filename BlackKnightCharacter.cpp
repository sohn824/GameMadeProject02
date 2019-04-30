// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackKnightCharacter.h"
#include "BlackKnightAIController.h"
#include "GameMadeProjectCharacter.h"
#include "ZombieCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ABlackKnightCharacter::ABlackKnightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_BlackKnightMesh(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden"));

	GetMesh()->SetSkeletalMesh(SK_BlackKnightMesh.Object);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(140.0f); //�� ���� ������Ʈ�� �þ� Ž�� �ޱ� ����
	PawnSensingComponent->SightRadius = 1000.0f; //�þ� Ž�� �ݰ��� ������ = 1000cm

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE1(TEXT("/Game/GameMadeProject/Characters/NPC/BlackKnight/Attack1_Montage.Attack1_Montage"));

	if (ATTACK_MONTAGE1.Succeeded())
	{
		AttackAnimMontage1 = ATTACK_MONTAGE1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE2(TEXT("/Game/GameMadeProject/Characters/NPC/BlackKnight/Attack2_Montage.Attack2_Montage"));

	if (ATTACK_MONTAGE2.Succeeded())
	{
		AttackAnimMontage2 = ATTACK_MONTAGE2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE3(TEXT("/Game/GameMadeProject/Characters/NPC/BlackKnight/Attack3_Montage.Attack3_Montage"));

	if (ATTACK_MONTAGE3.Succeeded())
	{
		AttackAnimMontage3 = ATTACK_MONTAGE3.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		CASTING_MONTAGE(TEXT("/Game/GameMadeProject/Characters/NPC/BlackKnight/Casting_Montage.Casting_Montage"));

	if (CASTING_MONTAGE.Succeeded())
	{
		CastingAnimMontage = CASTING_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DEAD_MONTAGE(TEXT("/Game/GameMadeProject/Characters/NPC/BlackKnight/Dead_Montage.Dead_Montage"));

	if (DEAD_MONTAGE.Succeeded())
	{
		DeadAnimMontage = DEAD_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FLOORBREAK_MONTAGE(TEXT("/Game/GameMadeProject/Characters/NPC/BlackKnight/FloorBreak_Montage.FloorBreak_Montage"));

	if (FLOORBREAK_MONTAGE.Succeeded())
	{
		FloorBreakAnimMontage = FLOORBREAK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		FLOORBREAK_EFFECT(TEXT("/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Ausar/P_AU_ShieldSlam_AOE.P_AU_ShieldSlam_AOE"));

	if (FLOORBREAK_EFFECT.Succeeded())
	{
		FloorBreakEffect = FLOORBREAK_EFFECT.Object;
	}


	bSensedTarget = false;

	//AI Controller ����
	AIControllerClass = ABlackKnightAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BlackKnightHealth = 3000.0f;
	BlackKnightMaxHealth = 3000.0f;
	BlackKnightDamage = 5;

	IsGroggy = false;
	CanGroggy = true;

}

// Called when the game starts or when spawned
void ABlackKnightCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &ABlackKnightCharacter::OnSeePlayer);
		//BeginPlay���� PawnSensingComponent�� OnSeePawn�̺�Ʈ�� �Ͼ�ٸ� OnSeePlayer �Լ��� ������ �� �ֵ��� �߰����ش�
	}
	
	FName WeaponSocket(TEXT("hand_rSocket"));
	BlackKnightWeapon = GetWorld()->SpawnActor<ABlackKnightWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (BlackKnightWeapon != nullptr)
	{
		BlackKnightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//�� ����Ʈ�� WeaponSocket�� BlackKnightWeapon ����

	}
}

// Called every frame
void ABlackKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > 4.0f) //������ Ÿ���� �ִ� ���¿��� 2�ʰ� Ÿ���� �ٽ� ã�� ���� ���
	{
		ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController());
		GetCharacterMovement()->MaxWalkSpeed = 150.0f; //�÷��̾ ��ĥ�� �ȱ��� �̵��ӵ�
		if (AIController)
		{
			bSensedTarget = false;
			AIController->SetEnemy(nullptr);
			//bSensedTarget boolean���� �������� CanSeePlayer ���� false�� �ǵ�����

			FStringClassReference MissPlayerRef(TEXT("/Game/GameMadeProject/UI/BlackKnightScripts/UI_MissPlayer.UI_MissPlayer_C"));
			if (UClass* MissPlayerClass = MissPlayerRef.TryLoadClass<UUserWidget>())
			{
				UUserWidget* MissPlayerPopup = CreateWidget<UUserWidget>(GetWorld(), MissPlayerClass);
				MissPlayerPopup->AddToViewport();
			}
		}
	}

}

// Called to bind functionality to input
void ABlackKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABlackKnightCharacter::OnSeePlayer(APawn* Pawn)
{
	ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController()); //�� ����Ʈ AI ��Ʈ�ѷ��� ����ȯ
	AGameMadeProjectCharacter* SensedPawn = Cast<AGameMadeProjectCharacter>(Pawn); //�Ű������� �÷��̾� ĳ���ͷ� ����ȯ

	LastSeenTime = GetWorld()->GetTimeSeconds(); //���������� �߰��� �ð� ������ �߰� ������ �ð��� ������

	GetCharacterMovement()->MaxWalkSpeed = 600.0f; //�÷��̾� �߽߰� �޸����� �̵��ӵ�
	if (AIController)
	{
		AIController->SetEnemy(SensedPawn); //Can See Player = true
	}

	if (bSensedTarget == false) //���� ���� -> ������ ��츸 UI ����
	{
		FStringClassReference FindPlayerRef(TEXT("/Game/GameMadeProject/UI/BlackKnightScripts/UI_FindPlayer.UI_FindPlayer_C"));
		if (UClass* FindPlayerClass = FindPlayerRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* FindPlayerPopup = CreateWidget<UUserWidget>(GetWorld(), FindPlayerClass);
			FindPlayerPopup->AddToViewport();
		}
	}

	bSensedTarget = true; //�÷��̾� ���� ������ �ǹ�
}

void ABlackKnightCharacter::BlackKnightAttack(AActor* HitActor)
{
	AGameMadeProjectCharacter* PlayerPawn = Cast<AGameMadeProjectCharacter>(HitActor); //���� �����߿� �÷��̾� ĳ���͸� ����ȯ ���� ������ �����ص�

	if (HitActor && HitActor != this && BlackKnightHealth > 0.0f)
	{
		int32 seed = FMath::RandRange(1, 3);
		switch (seed)
		{
		case 1:
			PlayAnimMontage(AttackAnimMontage1);
			break;
		case 2:
			PlayAnimMontage(AttackAnimMontage2);
			break;
		case 3:
			PlayAnimMontage(AttackAnimMontage3);
			break;
		}
	}
}


//AnimNotify ���� �Լ�
void ABlackKnightCharacter::ExecuteDamage(AActor* HitActor)
{
	FPointDamageEvent DamageEvent; //������ �̺�Ʈ ����� ���� ���� ����

	HitActor->TakeDamage(BlackKnightDamage, DamageEvent, GetController(), this);
	//������ ��, ������ ����, �������� �ִ� ��Ʈ�ѷ�, �������� �ִ� ��ü��
}

void ABlackKnightCharacter::EnterGroggy()
{
	FTimerHandle TimerHandle_Groggy;
	ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController()); //�� ����Ʈ AI ��Ʈ�ѷ��� ����ȯ
	AIController->EnterGroggy(); //������ Ű �� true�� �ٲٴ� �Լ�
	IsGroggy = true; //�ִϸ��̼� BP�� ������ ���� (�׷α� �ִϸ��̼� ���)
	CanGroggy = false; //�� �̻� �׷α� �Ұ���
	FStringClassReference EnterGroggyRef(TEXT("/Game/GameMadeProject/UI/BlackKnightScripts/UI_EnterGroggy.UI_EnterGroggy_C"));
	if (UClass* EnterGroggyClass = EnterGroggyRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* EnterGroggyPopup = CreateWidget<UUserWidget>(GetWorld(), EnterGroggyClass);
		EnterGroggyPopup->AddToViewport();
	}
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(TimerHandle_Groggy, this, &ABlackKnightCharacter::AfterGroggy, 8.0f, false);
	//8�� �� �׷α� �� ���� ����
}

void ABlackKnightCharacter::AfterGroggy()
{
	FStringClassReference AfterGroggyRef(TEXT("/Game/GameMadeProject/UI/BlackKnightScripts/UI_AfterGroggy.UI_AfterGroggy_C"));
	ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController()); //�� ����Ʈ AI ��Ʈ�ѷ��� ����ȯ
	if (UClass* AfterGroggyClass = AfterGroggyRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* AfterGroggyPopup = CreateWidget<UUserWidget>(GetWorld(), AfterGroggyClass);
		AfterGroggyPopup->AddToViewport();
	}
	IsGroggy = false; //�׷α� �ִϸ��̼� ��� ����
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	AIController->AfterGroggy();

	BlackKnightDamage *= 2; //������ ���
}

void ABlackKnightCharacter::SetCanUseSkill()
{
	ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController()); //�� ����Ʈ AI ��Ʈ�ѷ��� ����ȯ
	AIController->SetCanUseSkill();
}

void ABlackKnightCharacter::UseFloorBreak()
{
	PlayAnimMontage(FloorBreakAnimMontage, 1.0f);
	FStringClassReference FloorBreakRef(TEXT("/Game/GameMadeProject/UI/BlackKnightScripts/UI_FloorBreak.UI_FloorBreak_C"));
	if (UClass* FloorBreakClass = FloorBreakRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* FloorBreakPopup = CreateWidget<UUserWidget>(GetWorld(), FloorBreakClass);
		FloorBreakPopup->AddToViewport();
	}
}
