// Fill out your copyright notice in the Description page of Project Settings.
// EnemyComponent.cpp

#include "EnemyComponent.h"

#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

UEnemyComponent::UEnemyComponent() {
}

void UEnemyComponent::BeginPlay() {
    OwnerPawn = Cast<APawn>(GetOwner());
    HealthComponent = GetOwner()->GetComponentByClass<UHealthComponent>();
    CharacterMovementComponent = Cast<ACharacter>(GetOwner())->GetCharacterMovement();
    CharacterMovementComponent->MaxWalkSpeed = 250.f;
    CharacterMovementComponent->MinAnalogWalkSpeed = 10.f;
    CharacterMovementComponent->BrakingDecelerationWalking = 1000.f;
    CharacterMovementComponent->BrakingDecelerationFalling = 750.0f;

    // Cria o componente. Parece o Instantiate da Unity
    PawnSensingComponent = GetOwner()->GetComponentByClass<UPawnSensingComponent>();

    // Esse componente tem um evento chamado OnSeePawn que é chamado quando o
    // inimigo vê o jogador Adiciona um método para ser chamado quando o evento
    // OnSeePawn é chamado
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &UEnemyComponent::PursuitPawn);
    HealthComponent->OnDeath.AddDynamic(this, &UEnemyComponent::OnDeath);
}

void UEnemyComponent::OnComponentDestroyed(bool bDestroyingHierarchy) {
    if (PawnSensingComponent) {
        PawnSensingComponent->OnSeePawn.RemoveDynamic(this, &UEnemyComponent::PursuitPawn);
    }

    if (HealthComponent) {
        HealthComponent->OnDeath.RemoveDynamic(this, &UEnemyComponent::OnDeath);
    }
}

void UEnemyComponent::PursuitPawn(APawn *TargetPawn) {
    AAIController *AIController = Cast<AAIController>(OwnerPawn->GetController());
    if (AIController && TargetPawn) {
        AIController->MoveToActor(TargetPawn);
    }
}

void UEnemyComponent::OnDeath() {
    UE_LOG(LogTemp, Warning, TEXT("Enemy %s has died!"), *GetName());

    CharacterMovementComponent->MaxWalkSpeed = 0;
    CharacterMovementComponent->MinAnalogWalkSpeed = 0;
    CharacterMovementComponent->BrakingDecelerationWalking = 0;
    CharacterMovementComponent->BrakingDecelerationFalling = 0;
}
