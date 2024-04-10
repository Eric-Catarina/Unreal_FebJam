// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "MonoGrinding/HealthComponent.h"
#include "AllyComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UAllyComponent : public UActorComponent {
    GENERATED_BODY()

public:
    const std::string Category = "Ally";

    UAllyComponent();

    UFUNCTION(BlueprintCallable, Category = Category)
    void Enable();

    UFUNCTION(BlueprintCallable, Category = Category)
    void Disable();

    UFUNCTION(BlueprintCallable, Category = Category)
    void Enlist(AActor *LeaderP);

    UFUNCTION(BlueprintCallable, Category = Category)
    void MoveTo(const FVector &TargetLocation);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Category)
    AActor *Leader;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Category)
    ACharacter *OwnerCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Category)
    UHealthComponent *HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Category)
    bool Enabled;

protected:
    virtual void BeginPlay() override;
};
