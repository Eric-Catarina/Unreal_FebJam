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

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedUnitTemplateChanged,
                                            UUnitTemplate *,
                                            NewValue);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAlliesChanged);

UENUM(BlueprintType)
enum class EMoveAlliesResultType : uint8 {
    NoHit UMETA(DisplayName = "NoHit"),
    Success UMETA(DisplayName = "Success"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class MONOGRINDING_API ADefaultPlayer : public AMonoGrindingCharacter {
    GENERATED_BODY()

public:
    void RegenMana(int Amount);
    bool UseMana(int Amount);
    bool CheckHasEnoughMana(int Amount);

    UFUNCTION(BlueprintCallable, Category = "Player")
    void Move(FVector2D MovementVector);

    UFUNCTION(BlueprintCallable, Category = "Player")
    EMoveAlliesResultType MoveAllies();

    UFUNCTION(BlueprintCallable, Category = "Player")
    void SummonOrEnlistUnit();

    UFUNCTION(BlueprintCallable, Category = "Player")
    bool CreateUnitAtPosition(UUnitTemplate *Template, FVector Position);

    virtual void SetupPlayerInputComponent(
        class UInputComponent *PlayerInputComponent) override;

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

    UFUNCTION(BlueprintCallable, Category = "Player|Allies")
    void EnterSummoningMode(UUnitTemplate *UnitTemplate);

    FORCEINLINE class USpringArmComponent *GetCameraBoom() const {
        return CameraBoom;
    }

    FORCEINLINE class UCameraComponent *GetFollowCamera() const {
        return FollowCamera;
    }

    UPROPERTY(BlueprintAssignable, Category = "Player|Mana")
    FManaChanged ManaChanged;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ally")
    TArray<UAllyComponent *> Allies;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Debug")
    APlayerController *PlayerController;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ally")
    TArray<UUnitTemplate *> UnitTemplates;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

private:
    void BeginPlay() override;

    UFUNCTION(BlueprintCallable,
              Category = "Player|Allies",
              meta = (AllowPrivateAccess))
    bool CreateUnitFromSelectedTemplateAtLocation(FVector Position);

    UFUNCTION(BlueprintCallable,
              Category = "Player|Allies",
              meta = (AllowPrivateAccess))
    void DestroyUnitSummonIndicator();

    UFUNCTION(BlueprintCallable,
              Category = "Player|Allies",
              meta = (AllowPrivateAccess))
    void ClearSelectedUnitTemplate();

    void Enlist(ADefaultUnitOrchestrator *Unit);
    void SelectUnitTemplate(UUnitTemplate *Template);
    void SetMana(int Value);
    void OnSecondPassed();
    bool CursorRaycast(FHitResult &HitResult);

    UPROPERTY(BlueprintAssignable,
              Category = "Player|Allies",
              meta = (AllowPrivateAccess))
    FAlliesChanged AlliesChanged;

    UPROPERTY(BlueprintAssignable,
              Category = "Player|Allies",
              meta = (AllowPrivateAccess))
    FSelectedUnitTemplateChanged SelectedUnitTemplateChanged;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Player|Mana",
              meta = (AllowPrivateAccess = "true"))
    int MaxMana = 100;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Player|Mana",
              meta = (AllowPrivateAccess = "true"))
    int CurrentMana = MaxMana;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Player|Mana",
              meta = (AllowPrivateAccess = "true"))
    int ManaRegenPerSec = 1;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Player|Allies",
              meta = (AllowPrivateAccess = "true"))
    UUnitTemplate *SelectedUnitTemplate;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Player|Allies",
              meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AActor> UnitSummonIndicatorBlueprint;

    AActor *UnitSummonIndicator;
    FTimerHandle ManaRegenTimerHandle;
};
