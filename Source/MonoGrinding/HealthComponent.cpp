// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "MonoGrindingCharacter.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// Valores padrão
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
	if (CurrentHealth <= 0)
	{
		Die();
		UE_LOG(LogTemp, Warning, TEXT("HealthComponent: %s has died!"), *GetOwner()->GetName());
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
}

void UHealthComponent::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	CurrentHealth = MaxHealth;
}

void UHealthComponent::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = NewHealth;
}

void UHealthComponent::SetHealthPercent(float NewHealthPercent)
{
	CurrentHealth = FMath::Clamp(NewHealthPercent * MaxHealth, 0.f, MaxHealth);
}

void UHealthComponent::Die()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// Encontre o componente de malha esquelética
	USkeletalMeshComponent* MeshComp = Cast<USkeletalMeshComponent>(Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (MeshComp)
	{
		UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
		if (AnimInstance)
		{
			// Supondo que você tenha uma referência para a montagem de animação
            
			// Tocar a montagem de animação
			AnimInstance->Montage_Play(TPoseMontage);
			MeshComp->SetAnimationMode(EAnimationMode::AnimationCustomMode); // Opção para manter na pose final, mas depende do seu setup

			// Rotate the character 180 in X

		}
	}
	
	AMonoGrindingCharacter* OwnerCharacter = Cast<AMonoGrindingCharacter>(Owner);
	if (OwnerCharacter)
	{
		OwnerCharacter->Die();
	}

}



