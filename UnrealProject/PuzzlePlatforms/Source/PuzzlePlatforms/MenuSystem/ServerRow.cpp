// Troy Records Jr.


#include "ServerRow.h"

#include "Components/Button.h"
#include "PuzzlePlatforms/MenuSystem/MainMenu.h"

void UServerRow::Setup(UMainMenu* Parent, uint32 InIndex)
{
    MainMenu = Parent;
    Index = InIndex;
    ServerButton->OnClicked.AddDynamic(this, &UServerRow::OnClick);
}

void UServerRow::OnClick()
{
    MainMenu->SetIndex(Index);
    UE_LOG(LogTemp, Warning, TEXT("Clicked!"));
}
