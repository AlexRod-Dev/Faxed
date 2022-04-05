// Copyright Epic Games, Inc. All Rights Reserved.

#include "FaxedGameMode.h"
#include "FaxedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFaxedGameMode::AFaxedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
