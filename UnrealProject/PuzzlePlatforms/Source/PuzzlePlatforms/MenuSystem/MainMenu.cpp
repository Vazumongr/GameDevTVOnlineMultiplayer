// Troy Records Jr.


#include "MainMenu.h"
#include "Components/WidgetSwitcher.h"
#include "PuzzlePlatforms/MenuSystem/ServerRow.h"
#include "Components/TextBlock.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
    if(!ensure(ServerRowBPClass.Class != nullptr)) return;
    
    ServerRowClass = ServerRowBPClass.Class;
}

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

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
    UWorld* World = this->GetWorld();
    ensure(World);

    ServerList->ClearChildren();
    
    for(const FString& ServerName : ServerNames)
    {
        UServerRow* ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
        ensure(ServerRow);
        ServerRow->ServerName->SetText(FText::FromString(ServerName));
        ServerList->AddChild(ServerRow);
    }
    
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
        if(ensure(MenuInterface)) MenuInterface->RefreshServerList();
        
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
    if(MenuInterface != nullptr)
    {
        // if(!ensure(IPAddressField != nullptr)) return;
        // const FString& Address = IPAddressField->GetText().ToString();
        MenuInterface->Join(" ");
    }
}

void UMainMenu::QuitGame()
{
    UWorld* World = GetWorld();
    if(!ensure(World)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController)) return;

    PlayerController->ConsoleCommand(TEXT("quit"));
}
