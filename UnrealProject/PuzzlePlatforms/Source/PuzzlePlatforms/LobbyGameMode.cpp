// Troy Records Jr.


#include "LobbyGameMode.h"

#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if(++PlayerCount >= 2)
    {
        GetWorld()->GetTimerManager().SetTimer(TimeoutHandle, this, &ALobbyGameMode::Transition,10.f);
    }
    UE_LOG(LogTemp, Warning, TEXT("%i"), PlayerCount);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    --PlayerCount;
    UE_LOG(LogTemp, Warning, TEXT("%i"), PlayerCount);
}

void ALobbyGameMode::Transition()
{
    UWorld* World = GetWorld();
    bUseSeamlessTravel = true;

    UGameInstance* GameInstance = GetGameInstance();
    if(GameInstance == nullptr) return;
    if(UPuzzlePlatformsGameInstance* Instance = Cast<UPuzzlePlatformsGameInstance>(GameInstance))
    {
        Instance->StartSession();
    }
    
    if(World != nullptr) World->ServerTravel("/Game/Maps/ThirdPersonExampleMap");
}
