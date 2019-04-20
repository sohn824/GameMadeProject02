// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine.h"
#include "ZombieAIController.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //캡슐 컴포넌트의 크기 설정

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_ZombieMesh(TEXT("/Game/GameMadeProject/Characters/Enemy/Zombie01/warzombie_f_pedroso.warzombie_f_pedroso"));
	//좀비에 쓸 스켈레탈 메시 정보

	GetMesh()->SetSkeletalMesh(SK_ZombieMesh.Object);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(150.0f); //폰 센싱 컴포넌트의 시야 탐지 앵글 설정
	PawnSensingComponent->SightRadius = 1000.0f; //시야 탐지 반경의 반지름 = 1000cm

	bSensedTarget = false;

	AttackRangeComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackRange"));
	AttackRangeComponent->SetRelativeLocation(FVector(65.0f, 0.0f, 25.0f));
	AttackRangeComponent->InitCapsuleSize(20.0f, 70.0f);
	AttackRangeComponent->SetCollisionResponseToAllChannels(ECR_Ignore); //기본적으로 모든 콜리전에 대해 무시로 설정
	AttackRangeComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); //GameTraceChannel1 = 플레이어 콜리전에게만 Overlap으로 설정 (Config->DefaultEngine.ini에서 확인 가능)
	AttackRangeComponent->SetupAttachment(GetCapsuleComponent()); //좀비 캐릭터의 캡슐 컴포넌트에 공격 범위 컴포넌트를 붙여줬다

	AIControllerClass = AZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//AI 컨트롤러 지정부분

	ZombieHealth = 70.0f;

}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AZombieCharacter::OnSeePlayer);
		//BeginPlay에서 PawnSensingComponent의 OnSeePawn이벤트가 일어난다면 OnSeePlayer 함수가 구동될 수 있도록 추가해준다
	}
	//if (AttackRangeComponent)
	//{
	//	AttackRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombieCharacter::OnAttackRangeBeginOverlap);
	//	//공격범위 컴포넌트의 OnComponentBeginOverlap 함수에서 호출되도록 직접 만든 커스텀 BeginOverlap 함수를 연결해준다
	//}
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > 2.5f) //감지된 타겟이 있는 상태에서 2.5초간 타겟을 다시 찾지 못할 경우
	{
		AZombieAIController* AIController = Cast<AZombieAIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			AIController->SetEnemy(nullptr);
			//bSensedTarget boolean값과 블랙보드의 TargetEnemy 값을 다시 찾지 못한 상태로 되돌린다
		}
	}
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombieCharacter::OnSeePlayer(APawn* Pawn)
{
	AZombieAIController* AIController = Cast<AZombieAIController>(GetController()); //좀비 AI 컨트롤러로 형변환
	AGameMadeProjectCharacter* SensedPawn = Cast<AGameMadeProjectCharacter>(Pawn); //매개변수를 플레이어 캐릭터로 형변환

	LastSeenTime = GetWorld()->GetTimeSeconds(); //마지막으로 발견한 시간 변수에 발견 시점의 시간을 저장함
	bSensedTarget = true; //플레이어 감지 성공의 의미

	if (AIController)
	{
		AIController->SetEnemy(SensedPawn); //AIController에 구현해놓은 블랙보드 TargetEnemy 키값에 매개변수로 들어간 녀석을 설정하는 함수
	}
}

void AZombieCharacter::LookingForOverlapActor()
{
	TArray<AActor*> Overlaps;
	AttackRangeComponent->GetOverlappingActors(Overlaps, AGameMadeProjectCharacter::StaticClass()); //공격 범위 컴포넌트에 들어가 있는 액터들 중 플레이어 캐릭터만 Overlaps 배열에 추가

	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		AGameMadeProjectCharacter* OverlappingPawn = Cast<AGameMadeProjectCharacter>(Overlaps[i]); //배열의 원소를 플레이어 캐릭터로 형변환
		if (OverlappingPawn)
		{
			ExecuteMeleeDamage(OverlappingPawn); //밑의 함수 구현부 참조
		}
	}
}

void AZombieCharacter::OnAttackRangeBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ExecuteMeleeDamage(OtherActor); //아래의 데미지 전달 함수에 매개변수로 들어온 액터를 넘겨줌

	//FTimerHandle TimerHandle_Attack;
	//GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &AZombieCharacter::LookingForOverlapActor, 2.0f, true);
	////타이머를 이용하여 LookingForOverlapActor()함수가 2초마다 실행되도록 함
}

void AZombieCharacter::ExecuteMeleeDamage(AActor* HitActor)
{
	AGameMadeProjectCharacter* PlayerPawn = Cast<AGameMadeProjectCharacter>(HitActor); //들어온 액터중에 플레이어 캐릭터만 형변환 시켜 변수로 저장해둠

	if (HitActor && HitActor != this && ZombieHealth > 0.0f)
	{
		PlayAnimMontage(AttackAnimMontage, 1.5f); //공격 애님몽타주 실행

		float bAttackDuring = GetMesh()->AnimScriptInstance->Montage_GetPlayRate(AttackAnimMontage); //실행중이면 1, 아니면 0

		FPointDamageEvent DamageEvent; //데미지 이벤트 사용을 위한 변수 선언

		HitActor->TakeDamage(5, DamageEvent, GetController(), this);
		//데미지 양, 데미지 종류, 데미지를 주는 컨트롤러, 데미지를 주는 주체자
	}
}


