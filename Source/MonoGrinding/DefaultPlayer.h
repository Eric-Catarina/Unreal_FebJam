#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "MonoGrinding/AllyComponent.h"
#include "MonoGrinding/DefaultUnitOrchestrator.h"
#include "MonoGrinding/Unit.h"
#include "MonoGrindingCharacter.h"
#include "UObject/ObjectMacros.h"
#include "UnitTemplate.h"
#include <optional>

#include "DefaultPlayer.generated.h"

#define NAMEOF(variable) #variable

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FManaChanged, int, NewValue);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API ADefaultPlayer : public AMonoGrindingCharacter {
    GENERATED_BODY()

protected:
    bool TryUseMana(int Amount);

    UFUNCTION(BlueprintCallable, Category = "Player")
    void Move(FVector2D MovementVector);

    UFUNCTION(BlueprintCallable, Category = "Player")
    void MoveAllies();

    UFUNCTION(BlueprintCallable, Category = "Player")
    void SummonOrEnlistUnit();

    UFUNCTION(BlueprintCallable, Category = "Player")
    bool CreateUnitAtPosition(UUnitTemplate *Template, FVector Position);

    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    USpringArmComponent *CameraBoom;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    UCameraComponent *FollowCamera;

    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    UInputMappingContext *DefaultMappingContext;

public:
    ADefaultPlayer();

    void Tick(float DeltaTime) override;

    FORCEINLINE class USpringArmComponent *GetCameraBoom() const {
        return CameraBoom;
    }

    FORCEINLINE class UCameraComponent *GetFollowCamera() const {
        return FollowCamera;
    }

    UPROPERTY(BlueprintAssignable, Category = "Player|Mana")
    FManaChanged ManaChanged;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Mana")
    int MaxMana;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Mana")
    int CurrentMana;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ally")
    TArray<UAllyComponent *> Allies;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Debug")
    APlayerController *PlayerController;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ally")
    TArray<UUnitTemplate *> UnitTemplates;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

private:
    void BeginPlay();
    void Enlist(ADefaultUnitOrchestrator *Unit);
    void SelectUnitTemplate(UUnitTemplate *Template);

    UFUNCTION(BlueprintCallable, Category = "Player|Allies", meta = (AllowPrivateAccess = "true"))
    bool CreateUnitFromSelectedTemplateAtLocation(FVector Position);

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Player|Allies",
              meta = (AllowPrivateAccess = "true"))
    UUnitTemplate *SelectedUnitTemplate;
};
