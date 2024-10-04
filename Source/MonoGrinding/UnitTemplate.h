#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Unit.h"
#include "UnitTemplate.generated.h"

UCLASS()

class MONOGRINDING_API UUnitTemplate : public UPrimaryDataAsset {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    TSubclassOf<AActor> Blueprint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    int ManaCost = 1;
};
