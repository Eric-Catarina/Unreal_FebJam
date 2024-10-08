#pragma once

#include "CoreMinimal.h"
#include "MonoGrinding/AllyComponent.h"
#include "MonoGrinding/AttackComponent.h"
#include "MonoGrinding/EnemyComponent.h"
#include "MonoGrinding/Unit.h"
#include "MonoGrinding/UnitTemplate.h"
#include "DefaultUnitOrchestrator.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API ADefaultUnitOrchestrator : public AMonoGrindingCharacter,
                                                  public IUnit {
    GENERATED_BODY()

public:
    ADefaultUnitOrchestrator();

    void BeginPlay() override;
    const UUnitTemplate *GetTemplate() const;
    void Setup(UUnitTemplate *PTemplate);

    UFUNCTION(BlueprintCallable, Category = Category)
    bool SwitchToAlly();

    UFUNCTION(BlueprintCallable, Category = Category)
    bool SwitchToEnemy();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    UAllyComponent *AllyComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    UEnemyComponent *EnemyComponent;

private:
    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Unit",
              meta = (AllowPrivateAccess = "true"))
    UUnitTemplate *Template;
};
