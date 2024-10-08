// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "MonoGrindingCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() {
    PrimaryComponentTick.bCanEverTick = true;

    MaxHealth = 100.f;
    Current = MaxHealth;
}

void UHealthComponent::BeginPlay() {
    Super::BeginPlay();

    MovementComponent =
        GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
    SkeletalMeshComponent =
        GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
}

void UHealthComponent::TakeDamage(float DamageAmount) {
    SetCurrent(Current - DamageAmount);
}

void UHealthComponent::Heal(float HealAmount) {
    SetCurrent(Current + HealAmount);
}

void UHealthComponent::SetMax(float NewMaxHealth) {
    MaxHealth = NewMaxHealth;
    SetCurrent(MaxHealth);
}

void UHealthComponent::SetCurrent(float NewHealth) {
    if (IsDead)
        return;

    Current = FMath::Clamp(NewHealth, 0.f, MaxHealth);

    if (Current <= 0) {
        DieInternal();
    }

    float HealthPercent = GetPercent();
    HealthChangedPercent.Broadcast(HealthPercent);
}

void UHealthComponent::SetByPercent(float NewHealthPercent) {
    SetCurrent(NewHealthPercent * MaxHealth);
}

float UHealthComponent::GetPercent() const {
    return Current / MaxHealth;
}

void UHealthComponent::Die() {
    SetCurrent(0);
}

void UHealthComponent::DieInternal() {
    IsDead = true;
    OnDeath.Broadcast();

    if (SkeletalMeshComponent) {
        // UAnimInstance *AnimInstance = MeshComp->GetAnimInstance();
        // if (AnimInstance) {
        //         AnimInstance->Montage_Play(TPoseMontage);
        //     MeshComp->SetAnimationMode(EAnimationMode::AnimationCustomMode);
        //     //
        // }

        SkeletalMeshComponent->SetAnimationMode(
            EAnimationMode::AnimationCustomMode); //
    }

    if (MovementComponent) {
        MovementComponent->DisableMovement();
    }

    if (DeathSound) {
        UGameplayStatics::PlaySoundAtLocation(this, DeathSound,
                                              GetOwner()->GetActorLocation());
    }
}

void UHealthComponent::Revive() {
    ReviveInternal();
    SetCurrent(MaxHealth);
}

void UHealthComponent::ReviveInternal() {
    IsDead = false;

    if (MovementComponent) {
        MovementComponent->SetMovementMode(MOVE_Walking);
    }

    if (SkeletalMeshComponent) {
        SkeletalMeshComponent->SetAnimationMode(
            EAnimationMode::AnimationBlueprint);
    }
}
