// Troy Records Jr.


#include "MenuWidget.h"



void UMenuWidget::SetMenuInterface(IMenuInterface* InMenuInterface)
{
    MenuInterface = InMenuInterface;
}

void UMenuWidget::Setup()
{
    this->AddToViewport();
    this->bIsFocusable = true;
    
    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    UWorld* World = GetWorld();
    if(!ensure(World)) return;
    
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController)) return;
    
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::TearDown()
{
    this->RemoveFromViewport();
    this->bIsFocusable = false;

    FInputModeGameOnly InputModeData;
    InputModeData.SetConsumeCaptureMouseDown(false);

    UWorld* World = GetWorld();
    if(!ensure(World)) return;
    
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController)) return;
    
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = false;
    
}