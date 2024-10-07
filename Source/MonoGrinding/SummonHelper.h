#pragma once

#include "Engine/World.h"
#include "MonoGrinding/DefaultUnitOrchestrator.h"
#include "MonoGrinding/UnitTemplate.h"

static ADefaultUnitOrchestrator *
Summon(UUnitTemplate *Template,
       UWorld *World,
       FVector const &Location,
       FRotator const &Rotation,
       const FActorSpawnParameters &SpawnParameters = FActorSpawnParameters()) {
    ADefaultUnitOrchestrator *Unit = World->SpawnActor<ADefaultUnitOrchestrator>(
        Template->Blueprint, Location, Rotation, SpawnParameters);
    Unit->Setup(Template);

    return Unit;
}
