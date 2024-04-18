// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonoGrinding/HealthComponent.h"
#include "MonoGrindingCharacter.h"
#include "Perception/PawnSensingComponent.h"

#include "EnemyComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UEnemyComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UEnemyComponent();

    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime,
                               ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    void OnComponentDestroyed(bool bDestroyingHierarchy) override;

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void Enable();

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void Disable();

    UFUNCTION(Category = "Enemy")
    void StartPursuit();

    UFUNCTION(Category = "Enemy")
    void Pursuit();

    UFUNCTION(Category = "Enemy")
    void OnDeath();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    UMaterialInstance *EnemyMaterial;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Debug")
    UHealthComponent *HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Debug")
    UAttackComponent *AttackComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Debug")
    USkeletalMeshComponent *SkeletalMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Debug")
    AMonoGrindingCharacter *OwnerCustomCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Debug")
    bool Enabled;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Debug")
    UCharacterMovementComponent *CharacterMovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Debug")
    AActor *TargetActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Debug")
    AAIController *AiController;
};
