// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "MonoGrindingCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() {
    // Set this component to be initialized when the game starts, and to be ticked
    // every frame.  You can turn these features off to improve performance if you
    // don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // Valores padrão
    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
}

// Called when the game starts
void UHealthComponent::BeginPlay() {
    Super::BeginPlay();

    // ...

    OwnerActor = GetOwner();
    HealthBarWidget =
        Cast<UWidgetComponent>(OwnerActor->GetComponentByClass(UWidgetComponent::StaticClass()));
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UHealthComponent::TakeDamage(float DamageAmount) {
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
    SetCurrentHealth(CurrentHealth);
    if (CurrentHealth <= 0) {

        Die();
    }
}

void UHealthComponent::Heal(float HealAmount) {
    CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
    SetCurrentHealth(CurrentHealth);
}

void UHealthComponent::SetMaxHealth(float NewMaxHealth) {
    MaxHealth = NewMaxHealth;
    CurrentHealth = MaxHealth;
    SetCurrentHealth(CurrentHealth);
}

void UHealthComponent::SetCurrentHealth(float NewHealth) {
    CurrentHealth = NewHealth;

    float HealthPercent = GetHealthPercent();
    FString Command = FString::Printf(TEXT("UpdateHealthBar %f"), HealthPercent);
    FOutputDeviceNull Ar;

    HealthBarWidget->GetWidget()->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);
}

void UHealthComponent::SetHealthPercent(float NewHealthPercent) {
    CurrentHealth = FMath::Clamp(NewHealthPercent * MaxHealth, 0.f, MaxHealth);
    SetCurrentHealth(CurrentHealth);
}

float UHealthComponent::GetHealthPercent() const {
    return CurrentHealth / MaxHealth;
}

void UHealthComponent::Die() {
    // if (!Owner) return;
    // // Encontre o componente de malha esquelética
    // USkeletalMeshComponent* MeshComp =
    // Cast<USkeletalMeshComponent>(Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
    // if (MeshComp)
    // {
    // 	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
    // 	if (AnimInstance)
    // 	{
    // 		// Supondo que você tenha uma referência para a montagem de
    // animação
    //
    // 		// Tocar a montagem de animação
    // 		AnimInstance->Montage_Play(TPoseMontage);
    // 		MeshComp->SetAnimationMode(EAnimationMode::AnimationCustomMode); //
    // Opção para manter na pose final, mas depende do seu setup
    // 		// Rotate the character 180 in X
    // 	}
    // }

    AMonoGrindingCharacter *OwnerCharacter = Cast<AMonoGrindingCharacter>(OwnerActor);
    if (OwnerCharacter) {
        if (!DeathSound)
            return;
        UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetOwner()->GetActorLocation());
        OwnerCharacter->Die();
    }
}

void UHealthComponent::Revive() {
    // Play normal animations back
    if (!OwnerActor)
        return;
    USkeletalMeshComponent *MeshComp = Cast<USkeletalMeshComponent>(
        OwnerActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
    if (MeshComp) {
        MeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }
}
