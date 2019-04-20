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

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //ĸ�� ������Ʈ�� ũ�� ����

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_ZombieMesh(TEXT("/Game/GameMadeProject/Characters/Enemy/Zombie01/warzombie_f_pedroso.warzombie_f_pedroso"));
	//���� �� ���̷�Ż �޽� ����

	GetMesh()->SetSkeletalMesh(SK_ZombieMesh.Object);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(150.0f); //�� ���� ������Ʈ�� �þ� Ž�� �ޱ� ����
	PawnSensingComponent->SightRadius = 1000.0f; //�þ� Ž�� �ݰ��� ������ = 1000cm

	bSensedTarget = false;

	AttackRangeComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackRange"));
	AttackRangeComponent->SetRelativeLocation(FVector(65.0f, 0.0f, 25.0f));
	AttackRangeComponent->InitCapsuleSize(20.0f, 70.0f);
	AttackRangeComponent->SetCollisionResponseToAllChannels(ECR_Ignore); //�⺻������ ��� �ݸ����� ���� ���÷� ����
	AttackRangeComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); //GameTraceChannel1 = �÷��̾� �ݸ������Ը� Overlap���� ���� (Config->DefaultEngine.ini���� Ȯ�� ����)
	AttackRangeComponent->SetupAttachment(GetCapsuleComponent()); //���� ĳ������ ĸ�� ������Ʈ�� ���� ���� ������Ʈ�� �ٿ����

	AIControllerClass = AZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//AI ��Ʈ�ѷ� �����κ�

	ZombieHealth = 70.0f;

}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AZombieCharacter::OnSeePlayer);
		//BeginPlay���� PawnSensingComponent�� OnSeePawn�̺�Ʈ�� �Ͼ�ٸ� OnSeePlayer �Լ��� ������ �� �ֵ��� �߰����ش�
	}
	//if (AttackRangeComponent)
	//{
	//	AttackRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombieCharacter::OnAttackRangeBeginOverlap);
	//	//���ݹ��� ������Ʈ�� OnComponentBeginOverlap �Լ����� ȣ��ǵ��� ���� ���� Ŀ���� BeginOverlap �Լ��� �������ش�
	//}
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > 2.5f) //������ Ÿ���� �ִ� ���¿��� 2.5�ʰ� Ÿ���� �ٽ� ã�� ���� ���
	{
		AZombieAIController* AIController = Cast<AZombieAIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			AIController->SetEnemy(nullptr);
			//bSensedTarget boolean���� �������� TargetEnemy ���� �ٽ� ã�� ���� ���·� �ǵ�����
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
	AZombieAIController* AIController = Cast<AZombieAIController>(GetController()); //���� AI ��Ʈ�ѷ��� ����ȯ
	AGameMadeProjectCharacter* SensedPawn = Cast<AGameMadeProjectCharacter>(Pawn); //�Ű������� �÷��̾� ĳ���ͷ� ����ȯ

	LastSeenTime = GetWorld()->GetTimeSeconds(); //���������� �߰��� �ð� ������ �߰� ������ �ð��� ������
	bSensedTarget = true; //�÷��̾� ���� ������ �ǹ�

	if (AIController)
	{
		AIController->SetEnemy(SensedPawn); //AIController�� �����س��� ������ TargetEnemy Ű���� �Ű������� �� �༮�� �����ϴ� �Լ�
	}
}

void AZombieCharacter::LookingForOverlapActor()
{
	TArray<AActor*> Overlaps;
	AttackRangeComponent->GetOverlappingActors(Overlaps, AGameMadeProjectCharacter::StaticClass()); //���� ���� ������Ʈ�� �� �ִ� ���͵� �� �÷��̾� ĳ���͸� Overlaps �迭�� �߰�

	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		AGameMadeProjectCharacter* OverlappingPawn = Cast<AGameMadeProjectCharacter>(Overlaps[i]); //�迭�� ���Ҹ� �÷��̾� ĳ���ͷ� ����ȯ
		if (OverlappingPawn)
		{
			ExecuteMeleeDamage(OverlappingPawn); //���� �Լ� ������ ����
		}
	}
}

void AZombieCharacter::OnAttackRangeBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ExecuteMeleeDamage(OtherActor); //�Ʒ��� ������ ���� �Լ��� �Ű������� ���� ���͸� �Ѱ���

	//FTimerHandle TimerHandle_Attack;
	//GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &AZombieCharacter::LookingForOverlapActor, 2.0f, true);
	////Ÿ�̸Ӹ� �̿��Ͽ� LookingForOverlapActor()�Լ��� 2�ʸ��� ����ǵ��� ��
}

void AZombieCharacter::ExecuteMeleeDamage(AActor* HitActor)
{
	AGameMadeProjectCharacter* PlayerPawn = Cast<AGameMadeProjectCharacter>(HitActor); //���� �����߿� �÷��̾� ĳ���͸� ����ȯ ���� ������ �����ص�

	if (HitActor && HitActor != this && ZombieHealth > 0.0f)
	{
		PlayAnimMontage(AttackAnimMontage, 1.5f); //���� �ִԸ�Ÿ�� ����

		float bAttackDuring = GetMesh()->AnimScriptInstance->Montage_GetPlayRate(AttackAnimMontage); //�������̸� 1, �ƴϸ� 0

		FPointDamageEvent DamageEvent; //������ �̺�Ʈ ����� ���� ���� ����

		HitActor->TakeDamage(5, DamageEvent, GetController(), this);
		//������ ��, ������ ����, �������� �ִ� ��Ʈ�ѷ�, �������� �ִ� ��ü��
	}
}


