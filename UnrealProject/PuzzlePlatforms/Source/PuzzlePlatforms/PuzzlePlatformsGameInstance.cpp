// Troy Records Jr.


#include "PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSubsystem.h"

const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
    const ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetBP(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if(!ensure(MainMenuWidgetBP.Class)) return;
    MainMenuWidgetClass = MainMenuWidgetBP.Class;

    const ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetBP(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
    if(!ensure(InGameMenuWidgetBP.Class)) return;
    InGameMenuWidgetClass = InGameMenuWidgetBP.Class;
    
}

void UPuzzlePlatformsGameInstance::Init()
{
    if(ensure(MainMenuWidgetClass))
        UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MainMenuWidgetClass->GetName());
        
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if(Subsystem != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Subsytem found: %s"), *Subsystem->GetSubsystemName().ToString())
        SessionInterface = Subsystem->GetSessionInterface();
        if(SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Subsystem not found"));
    }
}

void UPuzzlePlatformsGameInstance::Host(FString ServerName)
{
    if(!SessionInterface.IsValid()) return;
    DesiredServerName = ServerName;
    auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
    if(ExistingSession != nullptr)
    {
        SessionInterface->DestroySession(SESSION_NAME);
    }
    else
    {
        CreateSession();
    }
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
    if(!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;
    
    if(MainMenuWidget != nullptr)
    {
        MainMenuWidget->TearDown();
    }

    SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
    
    
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(!ensure(PlayerController)) return;

    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if(SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
        //SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 100;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
    if(!ensure(MainMenuWidgetClass)) return;
    MainMenuWidget = CreateWidget<UMainMenu>(this, MainMenuWidgetClass);
        
    if(!ensure(MainMenuWidget)) return;

    MainMenuWidget->Setup();
    
    MainMenuWidget->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
    if(!ensure(InGameMenuWidgetClass)) return;
    UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuWidgetClass);

    if(!ensure(Menu)) return;

    Menu->Setup();

    Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
    if(!bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot create session."));
        return;
    }
    
    UEngine* Engine = GetEngine();
    if(!ensure(Engine != nullptr)) return;
    
    Engine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, TEXT("Hosting..."));

    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    World->ServerTravel("/Game/Maps/Lobby?listen");
    UE_LOG(LogTemp, Warning, TEXT("%s"), *SessionInterface->GetNamedSession(SESSION_NAME)->GetSessionIdStr());
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
    if(bSuccess)
    {
        CreateSession();
    }
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Finding session complete."));
    
    if(bWasSuccessful && SessionSearch.IsValid() && MainMenuWidget != nullptr)
    {
        TArray<FServerData> ServerDatas;
        for(const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            FServerData Data;
            Data.Name = SearchResult.GetSessionIdStr();
            Data.HostUserName = SearchResult.Session.OwningUserName;
            Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
            Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
            FString ServerName;
            if(SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
            {
                Data.Name = ServerName;
            }
            else
            {
                Data.Name = TEXT("Could not find name.");
            }
            
            ServerDatas.Add(Data);
        }

        MainMenuWidget->SetServerList(ServerDatas);
    }
    
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if(!SessionInterface.IsValid()) return;
    if(Result == EOnJoinSessionCompleteResult::Success)
    {
        FString Address;
        if(SessionInterface->GetResolvedConnectString(SessionName, Address))
        {
            
            APlayerController* PlayerController = GetFirstLocalPlayerController();
            if(!ensure(PlayerController)) return;

            PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
            
        }
        
    }
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
    if(!SessionInterface.IsValid()) return;
    FOnlineSessionSettings SessionSettings;
    IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? SessionSettings.bIsLANMatch = true : SessionSettings.bIsLANMatch = false;
    //SessionSettings.bIsLANMatch = false;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bUsesPresence = true;
    SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}
