// Copyright Epic Games, Inc. All Rights Reserved.

#include "FebGameJamGameMode.h"
#include "FebGameJamCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFebGameJamGameMode::AFebGameJamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
