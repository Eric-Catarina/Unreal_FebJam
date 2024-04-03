// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackComponent.h"

#include "EnemyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonoGrinding/DefaultPlayer.h"
#include "NiagaraFunctionLibrary.h"
#include "UnitComponent.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent() {
    // Set this component to be initialized when the game starts, and to be ticked
    // every frame.  You can turn these features off to improve performance if you
    // don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UAttackComponent::BeginPlay() {
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Attack, this,
                                           &UAttackComponent::PerformAttack, AttackInterval, true);
}

// Called every frame
void UAttackComponent::TickComponent(float DeltaTime,
                                     ELevelTick TickType,
                                     FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UAttackComponent::PerformAttack() {
    TArray<AActor *> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonoGrindingCharacter::StaticClass(),
                                          FoundActors); // Consider filtering by enemy class

    UE_LOG(LogTemp, Warning, TEXT("A"));
    AActor *OwnerActor = GetOwner();

    UHealthComponent *HealthComp = OwnerActor->FindComponentByClass<UHealthComponent>();
    if (!HealthComp || HealthComp->CurrentHealth <= 0)
        return;
    UE_LOG(LogTemp, Warning, TEXT("B"));

    AMonoGrindingCharacter *OwnerCharacter = Cast<AMonoGrindingCharacter>(OwnerActor);
    if (!OwnerCharacter) {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("C"));

    AActor *NearestTarget = nullptr;
    float NearestDistance = AttackRange;

    for (AActor *PossibleTargetActor : FoundActors) {
        if (!PossibleTargetActor->GetComponentByClass<UUnitComponent>() &&
            !PossibleTargetActor->IsA<ADefaultPlayer>())
            continue;

        float Distance = FVector::Distance(PossibleTargetActor->GetActorLocation(),
                                           GetOwner()->GetActorLocation());
        if (Distance >= NearestDistance)
            continue;

        NearestTarget = PossibleTargetActor;
        NearestDistance = Distance;
    }

    UE_LOG(LogTemp, Warning, TEXT("D"));
    if (!NearestTarget)
        return;

    UE_LOG(LogTemp, Warning, TEXT("E"));
    if (SlashVFX && SlashVFX2) {
        // Calcula a rotação do VFX para apontar para o alvo
        const FVector Direction =
            (NearestTarget->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
        const FRotator VFXRotation = Direction.Rotation();

        // Spawn do VFX na posição do alvo com a rotação calculada
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SlashVFX,
                                                       GetOwner()->GetActorLocation(), VFXRotation);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(), SlashVFX2, NearestTarget->GetActorLocation(), VFXRotation);
    }

    if (HitSound) {
        UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetOwner()->GetActorLocation());
    }

    DealDamage(NearestTarget);
}

void UAttackComponent::DealDamage(AActor *Target) {
    UE_LOG(LogTemp, Warning, TEXT("%f was dealed to %s"), AttackDamage, *Target->GetName());
    UGameplayStatics::ApplyDamage(Target, AttackDamage, GetOwner()->GetInstigatorController(),
                                  GetOwner(), nullptr);
}
