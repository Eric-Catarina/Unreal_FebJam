// Fill out your copyright notice in the Description page of Project Settings.

#include "MonoGrindingAlly.h"


#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMonoGrindingAlly::AMonoGrindingAlly()
{

}

void AMonoGrindingAlly::MoveToTargetLocation(const FVector& TargetLocation)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->MoveToLocation(TargetLocation);
	}
}


