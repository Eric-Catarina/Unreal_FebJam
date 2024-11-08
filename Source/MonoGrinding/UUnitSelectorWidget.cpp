#include "UUnitSelectorWidget.h"
#include "DefaultUnitOrchestrator.h"
#include "GameFramework/HUD.h"
#include "Math/MathFwd.h"
#include "MonoGrinding/NullHelpers.h"
#include "MonoGrinding/Unit.h"

void UUnitSelectorWidget::NativeConstruct() {
    Super::NativeConstruct();

    Hud = GetWorld()->GetFirstPlayerController()->GetHUD();
    PlayerController = GetWorld()->GetFirstPlayerController();
}

void UUnitSelectorWidget::OnReceiveDrawHud(int SizeX, int SizeY) {
    MG_RETURN_IF(!IsSelecting);

    float CurrentMousePositionX, CurrentMousePositionY;

    MG_RETURN_IF(
        !GetMousePosition(CurrentMousePositionX, CurrentMousePositionY));

    float RectWidth = CurrentMousePositionX - InitialMousePosition.X;
    float RectHeight = CurrentMousePositionY - InitialMousePosition.Y;
    Hud->DrawRect(SelectionColor, InitialMousePosition.X,
                  InitialMousePosition.Y, RectWidth, RectHeight);

    TArray<ADefaultUnitOrchestrator *> UnitsInSelectionRect;
    Hud->GetActorsInSelectionRectangle(
        InitialMousePosition,
        FVector2D(CurrentMousePositionX, CurrentMousePositionY),
        UnitsInSelectionRect, false);

    for (auto Unit : UnitsInSelectionRect) {
        MG_CONTINUE_IF(Unit->Team != ETeamType::Ally);
        SelectedUnits.AddUnique(Unit);
        Unit->Select();
    }

    for (auto Unit : SelectedUnits) {
        MG_CONTINUE_IF(UnitsInSelectionRect.Contains(Unit));
        Unit->Deselect();
    }
}

void UUnitSelectorWidget::SetSelectionState(bool Value) {
    IsSelecting = Value;

    float MousePositionX, MousePositionY;
    MG_RETURN_IF(!IsSelecting ||
                 !GetMousePosition(MousePositionX, MousePositionY));

    InitialMousePosition.Set(MousePositionX, MousePositionY);
}

bool UUnitSelectorWidget::GetMousePosition(float &X, float &Y) {
    return PlayerController->GetMousePosition(X, Y);
}
