// Fill out your copyright notice in the Description page of Project Settings.
// MonoGrindingEnemy.cpp

#include "MonoGrindingEnemy.h"

#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMonoGrindingEnemy::AMonoGrindingEnemy() {
    GetCharacterMovement()->MaxWalkSpeed = 250.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 10.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 750.0f;
    // Cria o componente. Parece o Instantiate da Unity
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

    // Esse componente tem um evento chamado OnSeePawn que é chamado quando o
    // inimigo vê o jogador Adiciona um método para ser chamado quando o evento
    // OnSeePawn é chamado
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AMonoGrindingEnemy::FollowPawn);

    TeamID = 1;
}

void AMonoGrindingEnemy::FollowPawn(APawn *TargetPawn) {
    AAIController *AIController = Cast<AAIController>(GetController());
    if (AIController && TargetPawn) {
        AIController->MoveToActor(TargetPawn);
    }
}

void AMonoGrindingEnemy::Die() {
    UE_LOG(LogTemp, Warning, TEXT("Enemy %s has died!"), *GetName());

    Super::Die();

    GetCharacterMovement()->MaxWalkSpeed = 0;
    GetCharacterMovement()->MinAnalogWalkSpeed = 0;
    GetCharacterMovement()->BrakingDecelerationWalking = 0;
    GetCharacterMovement()->BrakingDecelerationFalling = 0;
}

void AMonoGrindingEnemy::ReviveAsAlly() {

    // Restaurar a saúde
    HealthComponent->SetCurrentHealth(HealthComponent->MaxHealth);

    // Mudar o TeamID para 0
    this->TeamID = 0;

    // Atualizar a IA ou comportamentos para agir como aliado
    // ...
}
