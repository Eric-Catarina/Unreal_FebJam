#include "UnitSelectorWidget.h"
#include "GameFramework/HUD.h"
#include "MonoGrinding/NullHelpers.h"

void UUnitSelectorWidget::Tick(FGeometry MyGeometry, float InDeltaTime) {
    MG_RETURN_IF(!IsSelecting);

    AHUD *Hud = GetWorld()->GetFirstPlayerController()->GetHUD();
    Hud->DrawRect(SelectionColor, InitialMousePosition.X,
                  InitialMousePosition.X, float ScreenW, float ScreenH)
}

void UUnitSelectorWidget::SetSelectionState(bool value) {
    IsSelecting = value;
    float LocationX;
    float LocationY;
    if (IsSelecting && GetWorld()->GetFirstPlayerController()->GetMousePosition(
                           LocationX, LocationY)) {
        InitialMousePosition.Set(LocationX, LocationY);
    }
}
