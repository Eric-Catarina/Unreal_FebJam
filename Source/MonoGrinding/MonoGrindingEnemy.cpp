// Fill out your copyright notice in the Description page of Project Settings.
// MonoGrindingEnemy.cpp

#include "MonoGrindingEnemy.h"

#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMonoGrindingEnemy::AMonoGrindingEnemy()
{
	// Cria o componente. Parece o Instantiate da Unity
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	
	// Esse componente tem um evento chamado OnSeePawn que é chamado quando o inimigo vê o jogador
	// Adiciona um método para ser chamado quando o evento OnSeePawn é chamado
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMonoGrindingEnemy::FollowPawn);

}

void AMonoGrindingEnemy::FollowPawn(APawn* TargetPawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && TargetPawn)
	{
		AIController->MoveToActor(TargetPawn);
	}
}
