#include "UUnitSelectorWidget.h"
#include "GameFramework/HUD.h"
#include "MonoGrinding/NullHelpers.h"

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

    UE_LOG(LogTemp, Warning, TEXT("%fx%f / %fx%f"), InitialMousePosition.X,
           InitialMousePosition.Y, CurrentMousePositionX,
           CurrentMousePositionY);

    float RectWidth = CurrentMousePositionX - InitialMousePosition.X;
    float RectHeight = CurrentMousePositionY - InitialMousePosition.Y;
    Hud->DrawRect(SelectionColor, InitialMousePosition.X,
                  InitialMousePosition.Y, RectWidth, RectHeight);
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
