// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackKnightCharacter.h"
#include "BlackKnightAIController.h"
#include "GameMadeProjectCharacter.h"
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
	PawnSensingComponent->SetPeripheralVisionAngle(150.0f); //폰 센싱 컴포넌트의 시야 탐지 앵글 설정
	PawnSensingComponent->SightRadius = 1500.0f; //시야 탐지 반경의 반지름 = 1500cm

	bSensedTarget = false;

	//AI Controller 지정
	AIControllerClass = ABlackKnightAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BlackKnightHealth = 700.0f;

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
}

// Called every frame
void ABlackKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > 2.5f) //감지된 타겟이 있는 상태에서 2.5초간 타겟을 다시 찾지 못할 경우
	{
		ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			AIController->SetEnemy(nullptr);
			//bSensedTarget boolean값과 블랙보드의 CanSeePlayer 값을 false로 되돌린다
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
	bSensedTarget = true; //플레이어 감지 성공의 의미

	if (AIController)
	{
		AIController->SetEnemy(SensedPawn); //Can See Player = true
	}
}
