// Fill out your copyright notice in the Description page of Project Settings.
// EnemyComponent.cpp

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "MonoGrindingCharacter.h"
#include "Perception/PawnSensingComponent.h"

#include "EnemyComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UEnemyComponent : public UActorComponent {
    GENERATED_BODY()

public:
    const std::string EnemySpawnerCategory = "Enemy";

    UEnemyComponent();

    void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = Category)
    void Enable();

    UFUNCTION(BlueprintCallable, Category = Category)
    void Disable();

    UFUNCTION()
    void StartPursuit();

    UFUNCTION()
    void Pursuit(AActor *TargetActor);

    UFUNCTION()
    void OnDeath();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Category)
    bool Enabled;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Category)
    AMonoGrindingCharacter *OwnerCustomCharacter;

    UPROPERTY()
    UCharacterMovementComponent *CharacterMovementComponent;

    void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};
