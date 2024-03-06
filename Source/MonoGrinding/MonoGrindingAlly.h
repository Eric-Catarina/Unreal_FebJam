// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonoGrindingCharacter.h"

#include "MonoGrindingAlly.generated.h"

/**
 *
 */
UCLASS()
class MONOGRINDING_API AMonoGrindingAlly : public AMonoGrindingCharacter {
    GENERATED_BODY()
public:
    AMonoGrindingAlly();
    void MoveToTargetLocation(const FVector &TargetLocation);
};
