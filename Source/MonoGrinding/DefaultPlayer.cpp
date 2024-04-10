// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPlayer.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "EnemyComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "MonoGrinding/AllyComponent.h"

class UEnhancedInputLocalPlayerSubsystem;

ADefaultPlayer::ADefaultPlayer() {
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength =
        400.0f; // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
    CameraBoom->bDoCollisionTest = false;

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    // Attach the camera to the end of the boom and let the boom adjust to match
    // the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
    ObjectTypes.Add(
        UEngineTypes::ConvertToObjectType(ECC_Pawn)); // Ou o tipo customizado para suas unidades

    // Note: The skeletal mesh and anim blueprint references on the Mesh component
    // (inherited from Character) are set in the derived blueprint asset named
    // ThirdPersonCharacter (to avoid direct content references in C++)
}

void ADefaultPlayer::BeginPlay() {
    // Call the base class
    Super::BeginPlay();

    // Add Input Mapping Context
    if (APlayerController *PlayerController = Cast<APlayerController>(Controller)) {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                    PlayerController->GetLocalPlayer())) {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
            PlayerController->bShowMouseCursor = true;
            PlayerController->bEnableClickEvents = true;
            PlayerController->bEnableMouseOverEvents = true;
        }
    }
}

void ADefaultPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    UEnhancedInputComponent *EnhancedInputComponent =
        Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (!EnhancedInputComponent) {
        UE_LOG(LogTemplateCharacter, Error,
               TEXT("'%s' Failed to find an Enhanced Input component! This template "
                    "is built to use the Enhanced Input system. If you intend to use "
                    "the legacy system, then you will need to update this C++ file."),
               *GetNameSafe(this));
    }

    EnhancedInputComponent->BindAction(MoveAlliesAction, ETriggerEvent::Triggered, this,
                                       &ADefaultPlayer::MoveAllies);

    EnhancedInputComponent->BindAction(SummonAllyAction, ETriggerEvent::Triggered, this,
                                       &ADefaultPlayer::SummonOrEnlistUnit);

    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
                                       &ACharacter::StopJumping);

    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                                       &ADefaultPlayer::Move);

    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
                                       &ADefaultPlayer::Look);
}

void ADefaultPlayer::Move(const FInputActionValue &Value) {
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ADefaultPlayer::Look(const FInputActionValue &Value) {
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        // add yaw and pitch input to controller
        // AddControllerYawInput(LookAxisVector.X);
        // AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ADefaultPlayer::MoveAllies() {
    FHitResult HitResult;
    GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false,
                                                                    HitResult);
    FVector TargetLocation = HitResult.Location;
    UE_LOG(LogTemp, Warning, TEXT("Trying to Move Allies to %s"), *TargetLocation.ToString());

    if (!HitResult.bBlockingHit)
        return;

    UE_LOG(LogTemp, Warning, TEXT("Valid TargetLocation, Moving Allies"));

    for (auto &Ally : Allies) {
        Ally->MoveTo(TargetLocation);
    }
}

void ADefaultPlayer::SummonOrEnlistUnit() {
    UE_LOG(LogTemp, Warning, TEXT("Clicked Summon Ally"));

    FHitResult HitResult;
    GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(ObjectTypes, false,
                                                                              HitResult);
    FVector TargetLocation = HitResult.ImpactPoint;
    if (!HitResult.bBlockingHit)
        return;

    AActor *HitActor = HitResult.GetActor();

    if (!HitActor)
        return;

    UEnemyComponent *HitEnemy = HitActor->GetComponentByClass<UEnemyComponent>();

    if (HitEnemy) {
        HitActor->Destroy();
    }

    CreateAllyAtPosition(TargetLocation);
}

void ADefaultPlayer::CreateAllyAtPosition(FVector Position) {
    if (!GetWorld())
        return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Ajustar a posição baseada na posição atual do jogador
    FVector SpawnLocation = Position;

    // Define a rotação do aliado, se necessário. Neste exemplo, sem rotação.
    FRotator SpawnRotation = FRotator::ZeroRotator;

    // Criar a instância do aliado
    if (!AllyBlueprint) {
        UE_LOG(LogTemp, Warning, TEXT("AllyBlueprint is nullptr!"));
        return;
    }

    AActor *UnitActor =
        GetWorld()->SpawnActor<AActor>(AllyBlueprint, SpawnLocation, SpawnRotation, SpawnParams);

    if (!UnitActor)
        return;

    UAllyComponent *Ally = UnitActor->GetComponentByClass<UAllyComponent>();
    if (Ally) {
        Enlist(Ally);
    }
}

void ADefaultPlayer::Enlist(UAllyComponent *Ally) {
    Ally->Enlist(this);
    Cast<APawn>(Ally->GetOwner())->SpawnDefaultController();
    Allies.Add(Ally);
}
