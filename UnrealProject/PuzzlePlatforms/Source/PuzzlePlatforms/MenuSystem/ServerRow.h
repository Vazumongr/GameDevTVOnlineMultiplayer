// Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
    void Setup(class UMainMenu* Parent, uint32 Index);
    
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ServerName;
    UPROPERTY(meta = (BindWidget))
    class UButton* ServerButton;

private:
    UFUNCTION()
    void OnClick();

    UPROPERTY()
    class UMainMenu* MainMenu;
    uint32 Index;
};
