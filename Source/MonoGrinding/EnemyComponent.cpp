// Fill out your copyright notice in the Description page of Project Settings.
// EnemyComponent.cpp

#include "EnemyComponent.h"
#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "DefaultPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Logging/LogVerbosity.h"
#include "MonoGrinding/AttackComponent.h"
#include "MonoGrinding/MonoGrindingCharacter.h"

UEnemyComponent::UEnemyComponent() {
    OwnerCustomCharacter = Cast<AMonoGrindingCharacter>(GetOwner());
}

void UEnemyComponent::BeginPlay() {
    Super::BeginPlay();

    if (OwnerCustomCharacter && OwnerCustomCharacter->Team == ETeamType::Enemy) {
        Enable();
    }
}

void UEnemyComponent::OnComponentDestroyed(bool bDestroyingHierarchy) {
    Disable();
}

void UEnemyComponent::Enable() {
    Enabled = true;
    StartPursuit();

    UHealthComponent *HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
    if (HealthComponent) {
        HealthComponent->OnDeath.AddDynamic(this, &UEnemyComponent::OnDeath);
    }
}

void UEnemyComponent::Disable() {
    Enabled = false;

    UHealthComponent *HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
    if (HealthComponent) {
        HealthComponent->OnDeath.RemoveDynamic(this, &UEnemyComponent::OnDeath);
    }
}

void UEnemyComponent::StartPursuit() {
    TArray<AActor *> FoundPlayers;

    UWorld *World = GetOwner()->GetWorld();
    UE_LOG(LogTemp, Warning, TEXT("World: "), World);

    UGameplayStatics::GetAllActorsOfClass(World, ADefaultPlayer::StaticClass(), FoundPlayers);
    int FoundPlayersCount = FoundPlayers.Num();

    UE_LOG(LogTemp, Warning, TEXT("Found players: %d"), FoundPlayersCount);

    if (FoundPlayersCount > 0) {
        Pursuit(FoundPlayers[0]);
    }
}

void UEnemyComponent::Pursuit(AActor *TargetActor) {
    if (!TargetActor) {
        UE_LOG(LogTemp, Error, TEXT("TargetActor is null!"));
        return;
    }

    APawn *OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) {
        UE_LOG(LogTemp, Error, TEXT("OwnerPawn is null!"));
        return;
    }

    AController *Controller = OwnerPawn->GetController();
    if (!Controller) {
        UE_LOG(LogTemp, Error, TEXT("Controller is null!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Controller: %s"), Controller);

    AAIController *AIController = Cast<AAIController>(Controller);
    if (!AIController) {
        UE_LOG(LogTemp, Error, TEXT("AIController is null!"));
        return;
    }

    AIController->MoveToActor(TargetActor);
}

void UEnemyComponent::OnDeath() {
    UE_LOG(LogTemp, Warning, TEXT("Enemy %s has died!"), *GetName());
}

// CharacterMovementComponent = Cast<ACharacter>(GetOwner())->GetCharacterMovement();
// CharacterMovementComponent->MaxWalkSpeed = 250.f;
// CharacterMovementComponent->MinAnalogWalkSpeed = 10.f;
// CharacterMovementComponent->BrakingDecelerationWalking = 1000.f;
// CharacterMovementComponent->BrakingDecelerationFalling = 750.0f;
// CharacterMovementComponent->MaxWalkSpeed = 0;
// CharacterMovementComponent->MinAnalogWalkSpeed = 0;
// CharacterMovementComponent->BrakingDecelerationWalking = 0;
// CharacterMovementComponent->BrakingDecelerationFalling = 0;
