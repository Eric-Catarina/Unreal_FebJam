// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "HealthComponent.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChangedPercent, float, NewPercent);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UHealthComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UHealthComponent();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Die();

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Revive();

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FHealthChangedPercent HealthChangedPercent;

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeath OnDeath;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage *TPoseMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    USoundBase *DeathSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    USkeletalMeshComponent *SkeletalMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    UCharacterMovementComponent *MovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    bool IsDead = false;

protected:
    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetCurrent(float NewHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetMax(float NewMaxHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetPercent() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void DieInternal();

    UFUNCTION(BlueprintCallable, Category = "Health")
    void ReviveInternal();

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetByPercent(float NewHealthPercent);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float Current;
};
