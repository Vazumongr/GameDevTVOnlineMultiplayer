// Troy Records Jr.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton = nullptr;

private:
	UFUNCTION()
	void CancelPressed();
	UFUNCTION()
	void QuitPressed();
};
