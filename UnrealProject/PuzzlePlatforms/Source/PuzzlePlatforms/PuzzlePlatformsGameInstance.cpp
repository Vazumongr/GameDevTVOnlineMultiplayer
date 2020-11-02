// Troy Records Jr.


#include "PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

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
    FOnlineSessionSettings SessionSettings;
    SessionInterface->CreateSession(0, TEXT("My Session Game"), SessionSettings);
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(!ensure(PlayerController)) return;

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
    
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(!ensure(PlayerController)) return;

    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
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
}
