// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttackComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "Materials/MaterialInstance.h"
#include "MonoGrinding/HealthComponent.h"
#include "MonoGrinding/UnitTemplate.h"
#include "AllyComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UAllyComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UAllyComponent();

    void Setup(UUnitTemplate *PTemplate);

    UFUNCTION(BlueprintCallable, Category = "Ally")
    void Enable();

    UFUNCTION(BlueprintCallable, Category = "Ally")
    void Disable();

    UFUNCTION(BlueprintCallable, Category = "Ally")
    void Enlist(AActor *LeaderP);

    UFUNCTION(BlueprintCallable, Category = "Ally")
    void MoveTo(const FVector &TargetLocation);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ally")
    UMaterialInstance *AllyMaterial;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ally|Debug")
    USkeletalMeshComponent *SkeletalMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ally|Debug")
    AActor *Leader;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ally|Debug")
    ACharacter *OwnerCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ally|Debug")
    UAttackComponent *AttackComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ally|Debug")
    UHealthComponent *HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ally|Debug")
    bool Enabled;

private:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Ally")
    void OnDeath();

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Ally",
              meta = (AllowPrivateAccess))
    UUnitTemplate *Template;
};
