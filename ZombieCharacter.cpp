// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine.h"
#include "ZombieAIController.h"

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

	AIControllerClass = AZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//AI 컨트롤러 지정부분
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

