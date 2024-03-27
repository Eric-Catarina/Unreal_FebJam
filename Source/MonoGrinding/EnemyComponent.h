// Fill out your copyright notice in the Description page of Project Settings.
// EnemyComponent.cpp

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "MonoGrindingCharacter.h"
#include "Perception/PawnSensingComponent.h"

#include "EnemyComponent.generated.h"

UCLASS(ClassGroup = ("MonoGrinding"), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UEnemyComponent : public UActorComponent {
    GENERATED_BODY()
public:
    UEnemyComponent();

    void BeginPlay() override;

    void OnComponentDestroyed(bool bDestroyingHierarchy) override;

protected:
    UPROPERTY()
    APawn *OwnerPawn;

    UPROPERTY()
    UPawnSensingComponent *PawnSensingComponent;

    UPROPERTY()
    UCharacterMovementComponent *CharacterMovementComponent;

    UPROPERTY()
    UHealthComponent *HealthComponent;

    UFUNCTION()
    void PursuitPawn(APawn *TargetPawn);

    UFUNCTION()
    void OnDeath();
};
