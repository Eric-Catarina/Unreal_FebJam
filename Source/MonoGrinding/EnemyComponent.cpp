// Fill out your copyright notice in the Description page of Project Settings.

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
}

void UEnemyComponent::BeginPlay() {
    Super::BeginPlay();
    OwnerCustomCharacter = Cast<AMonoGrindingCharacter>(GetOwner());
    AttackComponent = GetOwner()->FindComponentByClass<UAttackComponent>();
    HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
    SkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

void UEnemyComponent::OnComponentDestroyed(bool bDestroyingHierarchy) {
    Disable();
}

void UEnemyComponent::Enable() {
    Enabled = true;
    StartPursuit();

    if (AttackComponent) {
        AttackComponent->TargetType = ETeamType::Ally;
    }

    if (HealthComponent) {
        HealthComponent->OnDeath.AddDynamic(this, &UEnemyComponent::OnDeath);
    }

    if (SkeletalMeshComponent && EnemyMaterial) {
        SkeletalMeshComponent->SetMaterial(0, EnemyMaterial);
    }
}

void UEnemyComponent::Disable() {
    Enabled = false;

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
