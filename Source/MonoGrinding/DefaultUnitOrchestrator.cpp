#include "DefaultUnitOrchestrator.h"
#include "MonoGrinding/AllyComponent.h"
#include "MonoGrinding/AttackComponent.h"
#include "MonoGrinding/EnemyComponent.h"
#include "MonoGrinding/MonoGrindingCharacter.h"

ADefaultUnitOrchestrator::ADefaultUnitOrchestrator() {
    AllyComponent = CreateDefaultSubobject<UAllyComponent>(TEXT("AllyComponent"));
    EnemyComponent = CreateDefaultSubobject<UEnemyComponent>(TEXT("EnemyComponent"));
}

void ADefaultUnitOrchestrator::BeginPlay() {
    Super::BeginPlay();
}

bool ADefaultUnitOrchestrator::SwitchToAlly() {
    if (Team == ETeamType::Ally || !HealthComponent)
        return false;

    bool IsAliveEnemy = Team == ETeamType::Enemy && !HealthComponent->IsDead;
    if (IsAliveEnemy)
        return false;

    if (AllyComponent) {
        AllyComponent->Enable();
    }

    if (EnemyComponent) {
        EnemyComponent->Disable();
    }

    Team = ETeamType::Ally;
    return true;
}

bool ADefaultUnitOrchestrator::SwitchToEnemy() {
    if (Team == ETeamType::Enemy)
        return false;

    if (AllyComponent) {
        AllyComponent->Disable();
    }

    if (EnemyComponent) {
        EnemyComponent->Enable();
    }

    Team = ETeamType::Enemy;
    return true;
}
