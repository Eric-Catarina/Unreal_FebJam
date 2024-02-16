// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MONOGRINDING_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this component's properties
	UAttackComponent();
	
	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackRange = 500.0f;

	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackDamage = 10.0f;
	
	UPROPERTY(EditAnywhere, Category="Team")
	int32 TeamID; // 0
	
	// Time handle for managing attack interval.
	FTimerHandle TimerHandle_Attack;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void PerformAttack();

private:
	void DealDamage(AActor* Target);

	bool IsEnemy(AActor* Target)
	{
		// Implement logic to determine if the target is an enemy based on TeamID
		// This could involve checking a similar TeamID property on the target actor/component
		return true;
	}
};
