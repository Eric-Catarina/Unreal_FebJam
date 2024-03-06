// Fill out your copyright notice in the Description page of Project Settings.

#include "MonoGrindingUnit.h"

// Fill out your copyright notice in the Description page of Project Settings.
// MonoGrindingEnemy.cpp

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AMonoGrindingUnit::AMonoGrindingUnit() {
    GetCharacterMovement()->MaxWalkSpeed = 250.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 10.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 750.0f;
    // Cria o componente. Parece o Instantiate da Unity
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

    // Esse componente tem um evento chamado OnSeePawn que é chamado quando o
    // inimigo vê o jogador Adiciona um método para ser chamado quando o evento
    // OnSeePawn é chamado
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AMonoGrindingUnit::FollowPawn);

    TeamID = 1;
}

void AMonoGrindingUnit::FollowPawn(APawn *TargetPawn) {
    if (TeamID == 0)
        return;
    AAIController *AIController = Cast<AAIController>(GetController());
    if (AIController && TargetPawn) {
        AIController->MoveToActor(TargetPawn);
    }
}

void AMonoGrindingUnit::Die() {
    UE_LOG(LogTemp, Warning, TEXT("Enemy %s has died!"), *GetName());

    Super::Die();
}

void AMonoGrindingUnit::ReviveAsAlly() {
    HealthComponent->SetCurrentHealth(HealthComponent->MaxHealth);

    this->TeamID = 0;
    Super::Revive();
}

bool AMonoGrindingUnit::TryReviveAsAlly() {
    if (HealthComponent->CurrentHealth <= 0 && TeamID == 1) {
        ReviveAsAlly();
        return true;
    }
    return false;
}

void AMonoGrindingUnit::MoveToTargetLocation(const FVector &TargetLocation) {
    AAIController *AIController = Cast<AAIController>(GetController());
    if (AIController) {
        UE_LOG(LogTemp, Warning, TEXT("Trying to move to location %s"), *TargetLocation.ToString());

        AIController->MoveToLocation(TargetLocation);
    }
}
