// Copyright Epic Games, Inc. All Rights Reserved.

#include "MonoGrindingCharacter.h"

#include "AttackComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "HealthComponent.h"
#include "Misc/OutputDeviceNull.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AMonoGrindingCharacter::AMonoGrindingCharacter() {
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the
    // camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate =
        FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->BrakingDecelerationFalling = 750.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
    GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
    GetCharacterMovement()->GravityScale = 1.75f;
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->MaxAcceleration = 1500;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 10.f;
    GetCharacterMovement()->SetFixedBrakingDistance(200.0f);
    GetCharacterMovement()->UseAccelerationForPathFollowing();
    GetCharacterMovement()->bUseSeparateBrakingFriction = true;

    HealthComponent =
        CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    AttackComponent =
        CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
}

void AMonoGrindingCharacter::BeginPlay() {
    Super::BeginPlay();
}

float AMonoGrindingCharacter::TakeDamage(float Damage,
                                         FDamageEvent const &DamageEvent,
                                         AController *EventInstigator,
                                         AActor *DamageCauser) {
    float processedDamage =
        Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    HealthComponent->TakeDamage(processedDamage);

    return processedDamage;
}

void AMonoGrindingCharacter::Die() {
    HealthComponent->Die();
}

void AMonoGrindingCharacter::Revive() {
    HealthComponent->Revive();
}
