// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"

#include "MonoGrindingEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent(): TeamID(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Attack, this, &UAttackComponent::PerformAttack, AttackInterval, true);
	
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackComponent::PerformAttack()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors); // Consider filtering by enemy class

	AActor* NearestTarget = nullptr;
	float NearestDist = AttackRange;

	for (AActor* Actor : FoundActors)
	{
		if (Actor->GetClass()->IsChildOf(AMonoGrindingEnemy::StaticClass())) // Ensure it's an enemy
		{
			
			float Dist = FVector::Distance(Actor->GetActorLocation(), GetOwner()->GetActorLocation());
			if (Dist < NearestDist)
			{
				NearestTarget = Actor;
				NearestDist = Dist;
			}
		}
	}

	if (NearestTarget)
	{
		DealDamage(NearestTarget);
	}
}

void UAttackComponent::DealDamage(AActor* Target)
{
	// Implement damage dealing logic here.
	// This could involve calling a TakeDamage method on the target actor, for example.
}