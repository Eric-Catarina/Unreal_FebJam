// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"

#include "MonoGrindingEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonoGrindingCharacter::StaticClass(), FoundActors); // Consider filtering by enemy class

	AActor* OwnerActor = GetOwner();
	AMonoGrindingCharacter* OwnerCharacter = Cast<AMonoGrindingCharacter>(OwnerActor);
	if (!OwnerCharacter)
	{
		return;
	}
	
	TeamID = OwnerCharacter->TeamID;
	AActor* NearestTarget = nullptr;
	float NearestDist = AttackRange;

	for (AActor* Actor : FoundActors)
	{
		AMonoGrindingCharacter* TargetCharacter = Cast<AMonoGrindingCharacter>(Actor);
		if (TargetCharacter && TargetCharacter->TeamID != TeamID)
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
	UE_LOG(LogTemp, Warning, TEXT("%f was dealed to %s"),AttackDamage,   *Target->GetName());
	UGameplayStatics::ApplyDamage(Target, AttackDamage, GetOwner()->GetInstigatorController(), GetOwner(), nullptr);
}