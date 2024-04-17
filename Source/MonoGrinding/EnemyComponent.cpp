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
    PrimaryComponentTick.bCanEverTick = true;

    APawn *OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn)
        return;

    AController *Controller = OwnerPawn->GetController();
    if (!Controller)
        return;

    AiController = Cast<AAIController>(Controller);
}

void UEnemyComponent::TickComponent(float DeltaTime,
                                    ELevelTick TickType,
                                    FActorComponentTickFunction *ThisTickFunction) {
    if (!Enabled)
        return;

    Pursuit();
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
        TargetActor = FoundPlayers[0];
    }
}

void UEnemyComponent::Pursuit() {
    if (!TargetActor) {
        UE_LOG(LogTemp, Error, TEXT("TargetActor is null!"));
        return;
    }

    AiController->MoveToActor(TargetActor);
}

void UEnemyComponent::OnDeath() {
    UE_LOG(LogTemp, Warning, TEXT("Enemy %s has died!"), *GetName());
}
