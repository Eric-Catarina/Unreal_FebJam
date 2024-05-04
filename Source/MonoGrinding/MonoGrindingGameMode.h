// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MonoGrinding/DefaultUnitOrchestrator.h"
#include "MonoGrindingGameMode.generated.h"

UCLASS(minimalapi)

class AMonoGrindingGameMode : public AGameModeBase {
    GENERATED_BODY()

public:
    AMonoGrindingGameMode();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawner")
    TSubclassOf<ADefaultUnitOrchestrator> DefaultEnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawner")
    FVector SpawnAreaCenter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawner")
    float SpawnAreaRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawner")
    float SpawnInterval;

private:
    FTimerHandle SpawnTimerHandle;

    void SpawnEnemy();
    void BeginPlay();
};
