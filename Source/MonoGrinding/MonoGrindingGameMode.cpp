#include "MonoGrindingGameMode.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Engine/World.h"
#include "MonoGrinding/NullHelpers.h"
#include "MonoGrinding/SummonHelper.h"
#include "MonoGrindingCharacter.h"
#include "NavigationSystem.h"
#include "UObject/ConstructorHelpers.h"

AMonoGrindingGameMode::AMonoGrindingGameMode() {
    PrimaryActorTick.bCanEverTick = false;
}

void AMonoGrindingGameMode::BeginPlay() {
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this,
                                    &AMonoGrindingGameMode::SpawnEnemy,
                                    SpawnInterval, true);
}

void AMonoGrindingGameMode::SpawnEnemy() {
    MG_RETURN_IF(!DefaultEnemyUnitTemplate);

    UNavigationSystemV1 *navSystem =
        UNavigationSystemV1::GetCurrent(GetWorld());
    FNavLocation navLocation;
    navSystem->GetRandomPointInNavigableRadius(SpawnAreaCenter, SpawnAreaRadius,
                                               navLocation);

    UE_LOG(LogTemp, Warning, TEXT("GameMode::SpawnEnemy -> Radius: %f"),
           SpawnAreaRadius);
    UE_LOG(LogTemp, Warning, TEXT("GameMode::SpawnEnemy -> EnemyClass: %s"),
           *DefaultEnemyUnitTemplate->GetName());

    ADefaultUnitOrchestrator *Unit =
        Summon(DefaultEnemyUnitTemplate, GetWorld(), navLocation.Location,
               FRotator::ZeroRotator);

    MG_RETURN_IF(!Unit);

    Unit->SwitchToEnemy();
}
