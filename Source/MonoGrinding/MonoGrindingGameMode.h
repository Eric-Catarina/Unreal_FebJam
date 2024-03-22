// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MonoGrindingGameMode.generated.h"

UCLASS(minimalapi)

class AMonoGrindingGameMode : public AGameModeBase {
    GENERATED_BODY()

public:
    AMonoGrindingGameMode();

    const std::string EnemySpawnerCategory = "Enemy Spawner";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemySpawnerCategory)
    TSubclassOf<AActor> DefaultEnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemySpawnerCategory)
    FVector SpawnAreaCenter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemySpawnerCategory)
    float SpawnAreaRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemySpawnerCategory)
    float SpawnInterval;

private:
    FTimerHandle SpawnTimerHandle;

    void SpawnEnemy();
    void BeginPlay();
};
