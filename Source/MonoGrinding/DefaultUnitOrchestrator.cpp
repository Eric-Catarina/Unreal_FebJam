#include "DefaultUnitOrchestrator.h"
#include "MonoGrinding/AllyComponent.h"
#include "MonoGrinding/AttackComponent.h"
#include "MonoGrinding/EnemyComponent.h"
#include "MonoGrinding/MonoGrindingCharacter.h"
#include "NullHelpers.h"

ADefaultUnitOrchestrator::ADefaultUnitOrchestrator() {
    AllyComponent =
        CreateDefaultSubobject<UAllyComponent>(TEXT("AllyComponent"));
    EnemyComponent =
        CreateDefaultSubobject<UEnemyComponent>(TEXT("EnemyComponent"));
}

void ADefaultUnitOrchestrator::BeginPlay() {
    Super::BeginPlay();
}

void ADefaultUnitOrchestrator::Setup(UUnitTemplate *PTemplate) {
    MG_RETURN_IF(Template != nullptr);

    Template = PTemplate;
    if (AllyComponent) {
        AllyComponent->Setup(Template);
    }
}

const UUnitTemplate *ADefaultUnitOrchestrator::GetTemplate() const {
    return Template;
}

bool ADefaultUnitOrchestrator::SwitchToAlly() {
    MG_RETURN_VALUE_IF(Team == ETeamType::Ally || !HealthComponent, false);

    bool IsAliveEnemy = Team == ETeamType::Enemy && !HealthComponent->IsDead;
    MG_RETURN_VALUE_IF(IsAliveEnemy, false);

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

void ADefaultUnitOrchestrator::Select() {
    MG_RETURN_IF(IsSelected);
    IsSelected = true;
    OnSelected.Broadcast();
}

void ADefaultUnitOrchestrator::Deselect() {
    MG_RETURN_IF(!IsSelected);
    IsSelected = false;
    OnDeselected.Broadcast();
}
