// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //Ä¸½¶ ÄÄÆ÷³ÍÆ®ÀÇ Å©±â ¼³Á¤

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_ZombieMesh(TEXT("/Game/GameMadeProject/Characters/Enemy/Zombie01/warzombie_f_pedroso.warzombie_f_pedroso"));
	//Á»ºñ¿¡ ¾µ ½ºÄÌ·¹Å» ¸Þ½Ã Á¤º¸

	GetMesh()->SetSkeletalMesh(SK_ZombieMesh.Object);
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

