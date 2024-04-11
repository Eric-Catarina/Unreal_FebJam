// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "Materials/MaterialInstance.h"
#include "MonoGrinding/HealthComponent.h"
#include "AllyComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API UAllyComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UAllyComponent();

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
    UHealthComponent *HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ally|Debug")
    bool Enabled;

protected:
    virtual void BeginPlay() override;
};
