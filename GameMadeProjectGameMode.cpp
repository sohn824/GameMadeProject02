// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GameMadeProjectGameMode.h"
#include "GameMadeProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameMadeProjectGameMode::AGameMadeProjectGameMode()
{
	//set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GameMadeProject/Blueprints/PlayerBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

