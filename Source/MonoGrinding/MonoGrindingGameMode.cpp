// Copyright Epic Games, Inc. All Rights Reserved.

#include "MonoGrindingGameMode.h"
#include "MonoGrindingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMonoGrindingGameMode::AMonoGrindingGameMode() {
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
        TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL) {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}
