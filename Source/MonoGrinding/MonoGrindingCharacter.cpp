// Copyright Epic Games, Inc. All Rights Reserved.

#include "MonoGrindingCharacter.h"

#include "HealthComponent.h"
#include "AttackComponent.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Blueprint/UserWidget.h"
#include "Misc/OutputDeviceNull.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMonoGrindingCharacter

AMonoGrindingCharacter::AMonoGrindingCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	
}


void AMonoGrindingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthBarWidget = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
	
}

float AMonoGrindingCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
	HealthComponent->TakeDamage(DamageApplied); // Supondo que HealthComponent é seu componente de saúde

	if(!HealthBarWidget) return DamageApplied;
	
	float HealthPercent = GetHealthPercent();
	FString Command = FString::Printf(TEXT("UpdateHealthBar %f"), HealthPercent);
	FOutputDeviceNull Ar;
	
	HealthBarWidget->GetWidget()->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);
  
	
	
	return DamageApplied;
}

float AMonoGrindingCharacter::GetHealthPercent() const
{
	UHealthComponent* HealthComp = FindComponentByClass<UHealthComponent>();
	if (HealthComp)
	{
		return HealthComp->CurrentHealth / HealthComp->MaxHealth;
	}

	return 0.0f; // Se não encontrou, retorna 0
}

// Die function
void AMonoGrindingCharacter::Die()
{
	GetCharacterMovement()->DisableMovement();
	isDead = true;
}
