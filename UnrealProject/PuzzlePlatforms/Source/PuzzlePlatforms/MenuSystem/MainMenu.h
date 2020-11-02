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
	virtual bool Initialize() override;
	
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

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
	class UWidgetSwitcher* MenuSwitcher = nullptr;
	UPROPERTY(meta = (BindWidget))
    class UWidget* JoinMenu = nullptr;
	UPROPERTY(meta = (BindWidget))
    class UWidget* MainMenu = nullptr;
    
    // User Input
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* InputIPBox = nullptr;

private:
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
};
