#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "DefaultUnitOrchestrator.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "Math/Color.h"
#include "Math/MathFwd.h"
#include "UUnitSelectorWidget.generated.h"

UCLASS()

class MONOGRINDING_API UUnitSelectorWidget : public UUserWidget {
    GENERATED_BODY()

private:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable,
              Category = "Unit Selector",
              meta = (AllowPrivateAccess))
    void SetSelectionState(bool Value);

    UFUNCTION(BlueprintCallable,
              Category = "Unit Selector",
              meta = (AllowPrivateAccess))
    void OnReceiveDrawHud(int SizeX, int SizeY);

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Unit Selector",
              meta = (AllowPrivateAccess))
    FLinearColor SelectionColor;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Unit Selector",
              meta = (AllowPrivateAccess))
    bool IsSelecting;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Unit Selector",
              meta = (AllowPrivateAccess))
    FVector2D InitialMousePosition;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Unit Selector",
              meta = (AllowPrivateAccess))
    TArray<ADefaultUnitOrchestrator *> SelectedUnits;

    bool GetMousePosition(float &X, float &Y);
    AHUD *Hud;
    APlayerController *PlayerController;
};
