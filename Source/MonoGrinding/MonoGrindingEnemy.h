// Fill out your copyright notice in the Description page of Project Settings.
// MonoGrindingEnemy.cpp

#pragma once

#include "CoreMinimal.h"
#include "MonoGrindingCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "MonoGrindingEnemy.generated.h"



/**
 * 
 */
UCLASS()
class MONOGRINDING_API AMonoGrindingEnemy : public AMonoGrindingCharacter
{
	GENERATED_BODY()
public:
	AMonoGrindingEnemy();
protected:
	UFUNCTION()
	void FollowPawn(APawn* TargetPawn);
	UPROPERTY()
	UPawnSensingComponent* PawnSensingComp;
};
