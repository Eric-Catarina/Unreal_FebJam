// Fill out your copyright notice in the Description page of Project Settings.

#include "MonoGrindingAlly.h"


#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMonoGrindingAlly::AMonoGrindingAlly()
{
	TeamID = 0;
}

void AMonoGrindingAlly::MoveToTargetLocation(const FVector& TargetLocation)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to move to location %s"), *TargetLocation.ToString());

		AIController->MoveToLocation(TargetLocation);
	}
}


