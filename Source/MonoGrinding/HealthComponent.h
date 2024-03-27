// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UHealthComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UHealthComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime,
                               ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetCurrentHealth(float NewHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetMaxHealth(float NewMaxHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealthPercent(float NewHealthPercent);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Die();

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Revive();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    UCharacterMovementComponent *MovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeath OnDeath;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage *TPoseMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UWidgetComponent *HealthBarWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    USoundBase *DeathSound;

private:
    bool IsDead = false;
};
