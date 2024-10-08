// Fill out your copyright notice in the Description page of Project Settings.

#include "AllyComponent.h"
#include "AIController.h"
#include "AttackComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "MonoGrinding/MonoGrindingCharacter.h"

UAllyComponent::UAllyComponent() {
}

void UAllyComponent::BeginPlay() {
    Super::BeginPlay();
    AttackComponent = GetOwner()->FindComponentByClass<UAttackComponent>();
    HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
    SkeletalMeshComponent =
        GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

void UAllyComponent::Enable() {
    Enabled = true;

    if (AttackComponent) {
        AttackComponent->TargetType = ETeamType::Enemy;
    }

    if (AllyMaterial) {
        SkeletalMeshComponent->SetMaterial(0, AllyMaterial);
    }

    if (HealthComponent) {
        HealthComponent->OnDeath.AddDynamic(this, &UAllyComponent::OnDeath);
    }
}

void UAllyComponent::Disable() {
    Enabled = false;
}

void UAllyComponent::Enlist(AActor *LeaderP) {
    Leader = LeaderP;

    if (HealthComponent) {
        HealthComponent->Revive();
    }
}

void UAllyComponent::MoveTo(const FVector &TargetLocation) {
    UE_LOG(LogTemp, Warning, TEXT("Trying to move to location %s"),
           *TargetLocation.ToString());

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    AAIController *OwnerAiController =
        Cast<AAIController>(OwnerCharacter->GetController());

    if (!OwnerAiController) {
        UE_LOG(LogTemp, Warning,
               TEXT("AI Controller not properly configured, not moving"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("AI Controller all good, moving"));
    OwnerAiController->MoveToLocation(TargetLocation);
}

void UAllyComponent::OnDeath() {
    GetOwner()->Destroy();
}
