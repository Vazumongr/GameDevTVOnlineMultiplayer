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

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
    UWorld* World = this->GetWorld();
    ensure(World);

    ServerList->ClearChildren();

    uint32 i = 0;
    for(const FServerData& ServerData : ServerNames)
    {
        UServerRow* ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
        ensure(ServerRow);
        ServerRow->ServerName->SetText(FText::FromString(ServerData.Name));
        ServerRow->HostName->SetText(FText::FromString(ServerData.HostUserName));
        const FText ConnectionFraction = FText::FromString(FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers));
        ServerRow->ConnectionFraction->SetText(ConnectionFraction);
        ServerRow->Setup(this, i);
        ++i;
        ServerList->AddChild(ServerRow);
    }
    
}

void UMainMenu::SetIndex(uint32 InIndex)
{
    SelectedIndex = InIndex;
    UpdateServerRows();
}

void UMainMenu::UpdateServerRows()
{
    TArray<UWidget*> ChildWidgets = ServerList->GetAllChildren();
    
    for(int32 i = 0; i< ServerList->GetChildrenCount(); ++i)
    {
        if(UServerRow* ServerRow = Cast<UServerRow>(ServerList->GetChildAt(i)))
        {
            ServerRow->bSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
        }
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
    if(SelectedIndex.IsSet() && MenuInterface != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndex.GetValue());
        MenuInterface->Join(SelectedIndex.GetValue());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));
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
