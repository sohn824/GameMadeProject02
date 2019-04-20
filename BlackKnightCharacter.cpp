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
	PawnSensingComponent->SetPeripheralVisionAngle(150.0f); //�� ���� ������Ʈ�� �þ� Ž�� �ޱ� ����
	PawnSensingComponent->SightRadius = 1500.0f; //�þ� Ž�� �ݰ��� ������ = 1500cm

	bSensedTarget = false;

	//AI Controller ����
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
		//BeginPlay���� PawnSensingComponent�� OnSeePawn�̺�Ʈ�� �Ͼ�ٸ� OnSeePlayer �Լ��� ������ �� �ֵ��� �߰����ش�
	}
}

// Called every frame
void ABlackKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > 2.5f) //������ Ÿ���� �ִ� ���¿��� 2.5�ʰ� Ÿ���� �ٽ� ã�� ���� ���
	{
		ABlackKnightAIController* AIController = Cast<ABlackKnightAIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			AIController->SetEnemy(nullptr);
			//bSensedTarget boolean���� �������� CanSeePlayer ���� false�� �ǵ�����
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
	bSensedTarget = true; //�÷��̾� ���� ������ �ǹ�

	if (AIController)
	{
		AIController->SetEnemy(SensedPawn); //Can See Player = true
	}
}
