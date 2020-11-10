// Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Components/Button.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	
	virtual bool Initialize() override;
	
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	void SetServerList(TArray<FString> ServerNames);

	void SetIndex(uint32 InIndex);

protected:
    // Buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UButton* ConnectButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton = nullptr;

    // Menus
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
    class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
    class UWidget* MainMenu;
    
    // User Input
    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* ServerList;

	// Classes
	TSubclassOf<UUserWidget> ServerRowClass;

private:
	void UpdateServerRows();
	UFUNCTION()
	void Host();
	UFUNCTION()
	void OpenJoinMenu();
	UFUNCTION()
	void GoBack();
	UFUNCTION()
	void ConnectToGame();
	UFUNCTION()
	void QuitGame();

	TOptional<uint32> SelectedIndex;
};
