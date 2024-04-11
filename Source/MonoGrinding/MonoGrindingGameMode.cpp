// Copyright Epic Games, Inc. All Rights Reserved.

#include "MonoGrindingGameMode.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Engine/World.h"
#include "MonoGrindingCharacter.h"
#include "NavigationSystem.h"
#include "UObject/ConstructorHelpers.h"

AMonoGrindingGameMode::AMonoGrindingGameMode() {
    PrimaryActorTick.bCanEverTick = false;
}

void AMonoGrindingGameMode::BeginPlay() {
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("GameMode::BeginPlay"));
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AMonoGrindingGameMode::SpawnEnemy,
                                    SpawnInterval, true);
}

void AMonoGrindingGameMode::SpawnEnemy() {
    UE_LOG(LogTemp, Warning, TEXT("GameMode::SpawnEnemy"));
    if (!DefaultEnemyClass) {
        return;
    }

    UNavigationSystemV1 *navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    FNavLocation navLocation;
    navSystem->GetRandomPointInNavigableRadius(SpawnAreaCenter, SpawnAreaRadius, navLocation);

    UE_LOG(LogTemp, Warning, TEXT("GameMode::SpawnEnemy -> Radius: %f"), SpawnAreaRadius);
    UE_LOG(LogTemp, Warning, TEXT("GameMode::SpawnEnemy -> EnemyClass: %s"),
           *DefaultEnemyClass->GetName());

    ADefaultUnitOrchestrator *Unit = GetWorld()->SpawnActor<ADefaultUnitOrchestrator>(
        DefaultEnemyClass, navLocation.Location, FRotator::ZeroRotator);
    Unit->SwitchToEnemy();
}
