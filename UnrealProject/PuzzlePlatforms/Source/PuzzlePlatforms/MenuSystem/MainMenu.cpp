// Troy Records Jr.


#include "MainMenu.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    
    if(!Success) return false;

    if(!ensure(HostButton)) return false;
    HostButton->OnClicked.AddDynamic(this, &UMainMenu::Host);

    if(!ensure(JoinButton)) return false;
    JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
    
    if(!ensure(BackButton)) return false;
    BackButton->OnClicked.AddDynamic(this, &UMainMenu::GoBack);
    
    if(!ensure(ConnectButton)) return false;
    ConnectButton->OnClicked.AddDynamic(this, &UMainMenu::ConnectToGame);
    
    if(!ensure(QuitButton)) return false;
    QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

    return true;
}
void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
    TearDown();
}

void UMainMenu::Host()
{
    if(!ensure(MenuInterface)) return;
    MenuInterface->Host();
}

void UMainMenu::OpenJoinMenu()
{
    if(ensure(JoinMenu) && ensure(MenuSwitcher))
    {
        MenuSwitcher->SetActiveWidget(JoinMenu);
    }
        
}

void UMainMenu::GoBack()
{
    if(ensure(MainMenu) && ensure(MenuSwitcher))
    {
        MenuSwitcher->SetActiveWidget(MainMenu);
    }
}

void UMainMenu::ConnectToGame()
{
    if(!ensure(InputIPBox)) return;
    
    FString IPAddress = InputIPBox->GetText().ToString();
    
    if(ensure(MenuInterface))
        MenuInterface->Join(IPAddress);
}

void UMainMenu::QuitGame()
{
    UWorld* World = GetWorld();
    if(!ensure(World)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController)) return;

    PlayerController->ConsoleCommand(TEXT("quit"));
}
