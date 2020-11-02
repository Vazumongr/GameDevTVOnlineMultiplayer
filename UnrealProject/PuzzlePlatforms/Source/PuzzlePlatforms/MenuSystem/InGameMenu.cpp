// Troy Records Jr.


#include "InGameMenu.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

bool UInGameMenu::Initialize()
{
    bool Success = Super::Initialize();
    if(!Success) return false;

    if(!ensure(CancelButton)) return false;
    CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);

    if(!ensure(QuitButton)) return false;
    QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

    return true;
}

void UInGameMenu::CancelPressed()
{
    TearDown();
}

void UInGameMenu::QuitPressed()
{
    if(!ensure(MenuInterface)) return;
    MenuInterface->LoadMainMenu();
}
