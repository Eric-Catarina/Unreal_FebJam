// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "NiagaraSystem.h"

#include "AttackComponent.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8 {
    Enemy UMETA(DisplayName = "Enemy"),
    Ally UMETA(DisplayName = "Ally"),
    None UMETA(DisplayName = "None"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UAttackComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UAttackComponent();

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackInterval = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackRange = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackDamage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    UNiagaraSystem *SlashVFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    UNiagaraSystem *SlashVFX2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    FVector SlashVFXOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    USceneComponent *SlashVFXAttachPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    USoundBase *HitSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    ETeamType TargetType;

    // Time handle for managing attack interval.
    FTimerHandle TimerHandle_Attack;

    virtual void TickComponent(float DeltaTime,
                               ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    void PerformAttack();

protected:
    virtual void BeginPlay() override;
    void DealDamage(AActor *Target);
};
