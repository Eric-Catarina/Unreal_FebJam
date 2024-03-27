// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "MonoGrindingCharacter.h"
#include "Perception/PawnSensingComponent.h"

#include "UnitComponent.generated.h"

UCLASS(ClassGroup = ("MonoGrinding"), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UUnitComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UUnitComponent();

    void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    UHealthComponent *HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    ACharacter *OwnerCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    AActor *Leader;

    void Enlist(AActor *Leader);
    void MoveToTargetLocation(const FVector &TargetLocation);
};
