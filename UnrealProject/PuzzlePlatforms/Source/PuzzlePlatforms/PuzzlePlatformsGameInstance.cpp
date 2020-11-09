// Troy Records Jr.


#include "PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSubsystem.h"

const static FName SESSION_NAME = TEXT("My Session Game");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetBP(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if(!ensure(MainMenuWidgetBP.Class)) return;
    MainMenuWidgetClass = MainMenuWidgetBP.Class;

    ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetBP(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
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

void UPuzzlePlatformsGameInstance::Host()
{
    if(!SessionInterface.IsValid()) return;
    
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
        SessionSearch->bIsLanQuery = true;
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

    World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
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
        TArray<FString> ServerNames;
        
        for(const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            ServerNames.Add(SearchResult.GetSessionIdStr());
        }

        MainMenuWidget->SetServerList(ServerNames);
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
    SessionSettings.bIsLANMatch = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.NumPublicConnections = 2;
    SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}
