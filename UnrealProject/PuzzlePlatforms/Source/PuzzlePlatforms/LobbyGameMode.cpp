// Troy Records Jr.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if(++PlayerCount >= 3)
    {
        UWorld* World = GetWorld();
        bUseSeamlessTravel = true;
        if(World != nullptr) World->ServerTravel("/Game/Maps/ThirdPersonExampleMap");
    }
    UE_LOG(LogTemp, Warning, TEXT("%i"), PlayerCount);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    --PlayerCount;
    UE_LOG(LogTemp, Warning, TEXT("%i"), PlayerCount);
}
