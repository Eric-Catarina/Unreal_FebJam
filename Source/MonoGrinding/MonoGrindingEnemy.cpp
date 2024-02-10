// Fill out your copyright notice in the Description page of Project Settings.


#include "MonoGrindingEnemy.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AMonoGrindingEnemy::AMonoGrindingEnemy()
{
	if(CameraBoom)
	{
		// CameraBoom->SetActive(false);
		// CameraBoom->SetVisibility(false);
		CameraBoom->DestroyComponent();
	}
    if(FollowCamera)
    {
	    // FollowCamera->SetActive(false);
    	// FollowCamera->SetVisibility(false);
    	FollowCamera->DestroyComponent();
    }
}
