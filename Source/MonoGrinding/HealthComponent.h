// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"

#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MONOGRINDING_API UHealthComponent : public UActorComponent {
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UHealthComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);
    void SetMaxHealth(float NewMaxHealth);
    void SetCurrentHealth(float NewHealth);
    void SetHealthPercent(float NewHealthPercent);
    float GetHealthPercent() const;
    void Die();
    void Revive();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage *TPoseMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UWidgetComponent *HealthBarWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    USoundBase *DeathSound;

    AActor *OwnerActor;
};
