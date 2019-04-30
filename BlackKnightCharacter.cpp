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
	PawnSensingComponent->SetPeripheralVisionAngle(140.0f); //폰 센싱 컴포넌트의 시야 탐지 앵글 설정
	PawnSensingComponent->SightRadius = 1000.0f; //시야 탐지 반경의 반지름 = 1000cm

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

	//AI Controller 지정
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
		//BeginPlay에서 PawnSensingComponent의 OnSeePawn이벤트가 일어난다면 OnSeePlayer 함수가 구동될 수 있도록 추가해준다
	}
	
	FName WeaponSocket(TEXT("hand_rSocket"));
	BlackKnightWeapon = GetWorld()->SpawnActor<ABlackKnightWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (BlackKnightWeapon != nullptr)
	{
		BlackKnightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//블랙 나이트의 WeaponSocket에 BlackKnightWeapon 장착

	}
}

// Called every frame
void ABlackKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > 4.0f) //감지된 타겟이 있는 상태에서 2초간 타겟을 다시 찾지 못할 경우
	{
		ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController());
		GetCharacterMovement()->MaxWalkSpeed = 150.0f; //플레이어를 놓칠시 걷기의 이동속도
		if (AIController)
		{
			bSensedTarget = false;
			AIController->SetEnemy(nullptr);
			//bSensedTarget boolean값과 블랙보드의 CanSeePlayer 값을 false로 되돌린다

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
	ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController()); //블랙 나이트 AI 컨트롤러로 형변환
	AGameMadeProjectCharacter* SensedPawn = Cast<AGameMadeProjectCharacter>(Pawn); //매개변수를 플레이어 캐릭터로 형변환

	LastSeenTime = GetWorld()->GetTimeSeconds(); //마지막으로 발견한 시간 변수에 발견 시점의 시간을 저장함

	GetCharacterMovement()->MaxWalkSpeed = 600.0f; //플레이어 발견시 달리기의 이동속도
	if (AIController)
	{
		AIController->SetEnemy(SensedPawn); //Can See Player = true
	}

	if (bSensedTarget == false) //감지 실패 -> 성공일 경우만 UI 생성
	{
		FStringClassReference FindPlayerRef(TEXT("/Game/GameMadeProject/UI/BlackKnightScripts/UI_FindPlayer.UI_FindPlayer_C"));
		if (UClass* FindPlayerClass = FindPlayerRef.TryLoadClass<UUserWidget>())
		{
			UUserWidget* FindPlayerPopup = CreateWidget<UUserWidget>(GetWorld(), FindPlayerClass);
			FindPlayerPopup->AddToViewport();
		}
	}

	bSensedTarget = true; //플레이어 감지 성공의 의미
}

void ABlackKnightCharacter::BlackKnightAttack(AActor* HitActor)
{
	AGameMadeProjectCharacter* PlayerPawn = Cast<AGameMadeProjectCharacter>(HitActor); //들어온 액터중에 플레이어 캐릭터만 형변환 시켜 변수로 저장해둠

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


//AnimNotify 연결 함수
void ABlackKnightCharacter::ExecuteDamage(AActor* HitActor)
{
	FPointDamageEvent DamageEvent; //데미지 이벤트 사용을 위한 변수 선언

	HitActor->TakeDamage(BlackKnightDamage, DamageEvent, GetController(), this);
	//데미지 양, 데미지 종류, 데미지를 주는 컨트롤러, 데미지를 주는 주체자
}

void ABlackKnightCharacter::EnterGroggy()
{
	FTimerHandle TimerHandle_Groggy;
	ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController()); //블랙 나이트 AI 컨트롤러로 형변환
	AIController->EnterGroggy(); //블랙보드 키 값 true로 바꾸는 함수
	IsGroggy = true; //애니메이션 BP와 연결한 변수 (그로기 애니메이션 재생)
	CanGroggy = false; //더 이상 그로기 불가능
	FStringClassReference EnterGroggyRef(TEXT("/Game/GameMadeProject/UI/BlackKnightScripts/UI_EnterGroggy.UI_EnterGroggy_C"));
	if (UClass* EnterGroggyClass = EnterGroggyRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* EnterGroggyPopup = CreateWidget<UUserWidget>(GetWorld(), EnterGroggyClass);
		EnterGroggyPopup->AddToViewport();
	}
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(TimerHandle_Groggy, this, &ABlackKnightCharacter::AfterGroggy, 8.0f, false);
	//8초 뒤 그로기 후 상태 진입
}

void ABlackKnightCharacter::AfterGroggy()
{
	FStringClassReference AfterGroggyRef(TEXT("/Game/GameMadeProject/UI/BlackKnightScripts/UI_AfterGroggy.UI_AfterGroggy_C"));
	ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController()); //블랙 나이트 AI 컨트롤러로 형변환
	if (UClass* AfterGroggyClass = AfterGroggyRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* AfterGroggyPopup = CreateWidget<UUserWidget>(GetWorld(), AfterGroggyClass);
		AfterGroggyPopup->AddToViewport();
	}
	IsGroggy = false; //그로기 애니메이션 재생 중지
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	AIController->AfterGroggy();

	BlackKnightDamage *= 2; //데미지 상승
}

void ABlackKnightCharacter::SetCanUseSkill()
{
	ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController()); //블랙 나이트 AI 컨트롤러로 형변환
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
