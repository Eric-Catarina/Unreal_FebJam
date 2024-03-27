// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitComponent.h"
#include "GameFramework/Character.h"

// Fill out your copyright notice in the Description page of Project Settings.
// EnemyComponent.cpp

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UUnitComponent::UUnitComponent() {
}

void UUnitComponent::BeginPlay() {
    HealthComponent = GetOwner()->GetComponentByClass<UHealthComponent>();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    APawn *OwnerPawn = Cast<APawn>(GetOwner());

    if (!OwnerCharacter)
        return;

    UCharacterMovementComponent *CharacterMovementComponent =
        OwnerCharacter->GetCharacterMovement();
    CharacterMovementComponent->MaxWalkSpeed = 250.f;
    CharacterMovementComponent->MinAnalogWalkSpeed = 10.f;
    CharacterMovementComponent->BrakingDecelerationWalking = 1000.f;
    CharacterMovementComponent->BrakingDecelerationFalling = 750.0f;
}

void UUnitComponent::Enlist(AActor *LeaderParam) {
    Leader = LeaderParam;

    if (HealthComponent) {
        HealthComponent->SetCurrentHealth(HealthComponent->MaxHealth);
        HealthComponent->Revive();
    }
}

void UUnitComponent::MoveToTargetLocation(const FVector &TargetLocation) {
    UE_LOG(LogTemp, Warning, TEXT("Trying to move to location %s"), *TargetLocation.ToString());

    AAIController *OwnerAiController = Cast<AAIController>(OwnerCharacter->GetController());

    if (OwnerAiController) {
        OwnerAiController->MoveToLocation(TargetLocation);
    }
}
