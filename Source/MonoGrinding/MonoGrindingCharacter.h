// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AttackComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "Logging/LogMacros.h"
#include "MonoGrindingCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)

class AMonoGrindingCharacter : public ACharacter {
    GENERATED_BODY()

public:
    AMonoGrindingCharacter();
    virtual void BeginPlay() override;
    virtual float TakeDamage(float Damage,
                             FDamageEvent const &DamageEvent,
                             AController *EventInstigator,
                             AActor *DamageCauser) override;
    float GetHealthPercent() const;
    virtual void Die();
    void Revive();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    UHealthComponent *HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
    UAttackComponent *AttackComponent;
};
