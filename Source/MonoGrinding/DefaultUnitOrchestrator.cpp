// Fill out your copyright notice in the Description page of Project Settings.

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

void ADefaultUnitOrchestrator::SwitchToAlly() {
    if (AllyComponent) {
        AllyComponent->Enable();
    }

    if (EnemyComponent) {
        EnemyComponent->Disable();
    }

    Team = ETeamType::Ally;
}

void ADefaultUnitOrchestrator::SwitchToEnemy() {
    if (AllyComponent) {
        AllyComponent->Disable();
    }

    if (EnemyComponent) {
        EnemyComponent->Enable();
    }

    Team = ETeamType::Enemy;
}
