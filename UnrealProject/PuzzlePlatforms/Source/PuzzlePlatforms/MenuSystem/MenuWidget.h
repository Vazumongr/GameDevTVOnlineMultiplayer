// Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void Setup();
    void TearDown();
    void SetMenuInterface(IMenuInterface* MenuInterface);

protected:
    IMenuInterface* MenuInterface;
};
