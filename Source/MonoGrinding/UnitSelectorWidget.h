#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Math/Color.h"
#include "Math/MathFwd.h"
#include "UnitSelectorWidget.generated.h"

UCLASS()

class MONOGRINDING_API UUnitSelectorWidget : public UUserWidget {
    GENERATED_BODY()

protected:
    void Tick(FGeometry MyGeometry, float InDeltaTime);

private:
    UFUNCTION(BlueprintCallable,
              Category = "Unit Selector",
              meta = (AllowPrivateAccess))
    void SetSelectionState(bool value);

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
};
