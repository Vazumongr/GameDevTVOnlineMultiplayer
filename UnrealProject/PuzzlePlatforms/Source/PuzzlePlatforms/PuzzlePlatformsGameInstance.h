// Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();

	UFUNCTION(Exec)
	void Host() override;
	UFUNCTION(Exec)
	void Join(const FString& Address) override;
	UFUNCTION(BlueprintCallable)
	void LoadMainMenu() override;

	void RefreshServerList() override;
	
	UFUNCTION(BlueprintCallable)
	void LoadMenu();
	UFUNCTION(BlueprintCallable)
    void InGameLoadMenu();

	TSubclassOf<class UUserWidget> MainMenuWidgetClass;
	TSubclassOf<class UUserWidget> InGameMenuWidgetClass;

	

private:

	void OnCreateSessionComplete(FName Sessionname, bool bSuccess);
	void OnDestroySessionComplete(FName Sessionname, bool bSuccess);
	void OnFindSessionComplete(bool bWasSuccessful);

	void CreateSession();
	
	class UMainMenu* MainMenuWidget;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
