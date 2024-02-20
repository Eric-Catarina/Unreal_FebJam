// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonoGrindingCharacter.h"
#include "Perception/PawnSensingComponent.h"

#include "MonoGrindingUnit.generated.h"

/**
 * 
 */
UCLASS()
class MONOGRINDING_API AMonoGrindingUnit : public AMonoGrindingCharacter
{
	
	GENERATED_BODY()
	
public:
	AMonoGrindingUnit();
	
	UPROPERTY()
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void FollowPawn(APawn* TargetPawn);
	
	void ReviveAsAlly();
	bool TryReviveAsAlly();
	void MoveToTargetLocation(const FVector& TargetLocation);
	
protected:
	virtual void Die() override;
};
