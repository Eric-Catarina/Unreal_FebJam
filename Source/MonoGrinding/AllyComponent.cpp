// Fill out your copyright notice in the Description page of Project Settings.

#include "AllyComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UAllyComponent::UAllyComponent() {
    OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UAllyComponent::BeginPlay() {
    Super::BeginPlay();
    HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
}

void UAllyComponent::Enable() {
    Enabled = true;
}

void UAllyComponent::Disable() {
    Enabled = false;
}

void UAllyComponent::Enlist(AActor *LeaderP) {
    Leader = LeaderP;

    if (HealthComponent) {
        HealthComponent->SetCurrentHealth(HealthComponent->MaxHealth);
        HealthComponent->Revive();
    }
}

void UAllyComponent::MoveTo(const FVector &TargetLocation) {
    UE_LOG(LogTemp, Warning, TEXT("Trying to move to location %s"), *TargetLocation.ToString());

    AAIController *OwnerAiController = Cast<AAIController>(OwnerCharacter->GetController());

    if (OwnerAiController) {
        OwnerAiController->MoveToLocation(TargetLocation);
    }
}
