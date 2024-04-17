// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonoGrinding/AllyComponent.h"
#include "MonoGrinding/AttackComponent.h"
#include "MonoGrinding/EnemyComponent.h"
#include "DefaultUnitOrchestrator.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API ADefaultUnitOrchestrator : public AMonoGrindingCharacter {
    GENERATED_BODY()

public:
    ADefaultUnitOrchestrator();

    void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = Category)
    bool SwitchToAlly();

    UFUNCTION(BlueprintCallable, Category = Category)
    bool SwitchToEnemy();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    UAllyComponent *AllyComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    UEnemyComponent *EnemyComponent;
};
