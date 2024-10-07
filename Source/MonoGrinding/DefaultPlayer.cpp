#include "DefaultPlayer.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "EnemyComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "MonoGrinding/AllyComponent.h"
#include "MonoGrinding/DefaultUnitOrchestrator.h"
#include "MonoGrinding/SummonHelper.h"
#include "MonoGrinding/Unit.h"
#include "NullHelpers.h"

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

    MaxMana = 100;
    CurrentMana = MaxMana;
}

void ADefaultPlayer::Tick(float DeltaTime) {
    // if (!PlayerController)
    return;

    FHitResult HitResult;
    GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false,
                                                                    HitResult);

    FRotator LookAtRotation = (HitResult.Location - GetActorLocation()).Rotation();
    LookAtRotation.Pitch = 0;

    SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
}

void ADefaultPlayer::BeginPlay() {
    Super::BeginPlay();

    PlayerController = Cast<APlayerController>(Controller);
    if (!PlayerController) {
        return;
    }

    if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                PlayerController->GetLocalPlayer())) {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }

    if (UnitTemplates.Num() > 0 && UnitTemplates[0]) {
        {
            SelectUnitTemplate(UnitTemplates[0]);
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
}

void ADefaultPlayer::Move(FVector2D MovementVector) {
    if (!Controller) {
        return;
    }

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
        if (!Ally)
            continue;

        Ally->MoveTo(TargetLocation);
    }
}

void ADefaultPlayer::SummonOrEnlistUnit() {
    UE_LOG(LogTemp, Warning, TEXT("Clicked Summon Ally"));

    FHitResult HitResult;
    GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(ObjectTypes, false,
                                                                              HitResult);
    FVector TargetLocation = HitResult.ImpactPoint;

    MG_RETURN_IF(!HitResult.bBlockingHit);

    AActor *HitActor = HitResult.GetActor();

    MG_RETURN_IF(!HitActor);

    ADefaultUnitOrchestrator *HitUnit = Cast<ADefaultUnitOrchestrator>(HitActor);
    if (HitUnit) {
        Enlist(HitUnit);
    } else {
        CreateUnitFromSelectedTemplateAtLocation(TargetLocation);
    }
}

bool ADefaultPlayer::TryUseMana(int Amount) {
    if (Amount > CurrentMana) {
        UE_LOG(LogTemp, Warning, TEXT("Not enough mana"));
        return false;
    }

    CurrentMana -= Amount;
    UE_LOG(LogTemp, Warning, TEXT("Broadcasting ManaChanged"));
    ManaChanged.Broadcast(CurrentMana);

    return true;
}

bool ADefaultPlayer::CreateUnitFromSelectedTemplateAtLocation(FVector Position) {
    bool result = CreateUnitAtPosition(SelectedUnitTemplate, Position);
    return result;
}

bool ADefaultPlayer::CreateUnitAtPosition(UUnitTemplate *Template, FVector Position) {
    MG_LOG_TEMP_WARN_NULL_IF_RETURN_VALUE(GetWorld(), false);
    MG_LOG_TEMP_WARN_NULL_IF_RETURN_VALUE(Template, false);
    MG_LOG_TEMP_WARN_NULL_IF_RETURN_VALUE(Template->Blueprint, false);

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    FVector SpawnLocation = Position;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    ADefaultUnitOrchestrator *Unit =
        Summon(Template, GetWorld(), SpawnLocation, SpawnRotation, SpawnParams);

    MG_LOG_TEMP_WARN_NULL_IF_RETURN_VALUE(Unit, false);

    Enlist(Unit);
    return true;
}

void ADefaultPlayer::Enlist(ADefaultUnitOrchestrator *Unit) {
    MG_RETURN_IF(!Unit || !Unit->SwitchToAlly());

    UAllyComponent *Ally = Unit->AllyComponent;

    MG_RETURN_IF(!Ally);

    Ally->Enlist(this);
    Allies.Add(Ally);
    TryUseMana(Unit->GetTemplate()->ManaCost);
}

void ADefaultPlayer::SelectUnitTemplate(UUnitTemplate *Template) {
    SelectedUnitTemplate = Template;
}
