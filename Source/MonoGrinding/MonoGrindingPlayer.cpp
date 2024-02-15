// Fill out your copyright notice in the Description page of Project Settings.


#include "MonoGrindingPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MonoGrindingAlly.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

class UEnhancedInputLocalPlayerSubsystem;

AMonoGrindingPlayer::AMonoGrindingPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false;
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMonoGrindingPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMonoGrindingPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(MoveAlliesAction, ETriggerEvent::Triggered, this, &AMonoGrindingPlayer::MoveAllies);

		EnhancedInputComponent->BindAction(SummonAllyAction, ETriggerEvent::Triggered, this, &AMonoGrindingPlayer::SumonAlly);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMonoGrindingPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMonoGrindingPlayer::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMonoGrindingPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{

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

void AMonoGrindingPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		// AddControllerYawInput(LookAxisVector.X);
		// AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMonoGrindingPlayer::MoveAllies()
{
	

	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	FVector TargetLocation = HitResult.Location;
	if(HitResult.bBlockingHit)
	{
		for(auto& Ally : Allies)
		{
			Ally->MoveToTargetLocation(TargetLocation);
		}
	}
}
void AMonoGrindingPlayer::SumonAlly()
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked Summon Ally"));

	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	FVector TargetLocation = HitResult.Location;
	if(HitResult.bBlockingHit)
	{
		
		CreateAllyAtPosition(TargetLocation);
	}
}

void AMonoGrindingPlayer::CreateAllyAtPosition(FVector Position)
{
	if (GetWorld() == nullptr)
		return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	// Ajustar a posição baseada na posição atual do jogador
	FVector SpawnLocation = Position;
	
	// Define a rotação do aliado, se necessário. Neste exemplo, sem rotação.
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	// Criar a instância do aliado
	AMonoGrindingAlly* NewAlly = GetWorld()->SpawnActor<AMonoGrindingAlly>(AllyBlueprint, SpawnLocation, SpawnRotation, SpawnParams);

	// Adicione a lógica adicional aqui, como adicionar o aliado à lista de aliados
	if (NewAlly != nullptr)
	{
		NewAlly->SpawnDefaultController();
		Allies.Add(NewAlly);
	}
}

