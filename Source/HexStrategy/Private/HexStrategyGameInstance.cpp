#include "HexStrategyGameInstance.h"
#include "SaveGame/GameFileData.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/NavigationConfig.h"
#include "Types/SlateEnums.h"


void UHexStrategyGameInstance::Init()
{
	Super::Init();
	
	FSlateApplication::Get().GetNavigationConfig().Get().KeyEventRules.Add(EKeys::W, EUINavigation::Up);
	FSlateApplication::Get().GetNavigationConfig().Get().KeyEventRules.Add(EKeys::A, EUINavigation::Left);
	FSlateApplication::Get().GetNavigationConfig().Get().KeyEventRules.Add(EKeys::S, EUINavigation::Down);
	FSlateApplication::Get().GetNavigationConfig().Get().KeyEventRules.Add(EKeys::D, EUINavigation::Right);
}

void UHexStrategyGameInstance::UpdateLocalPlayerSaveSlot(const FString& newSlot)
{
	LocalPlayerSavelot = newSlot;
	UpdatePlayerDataTimeStamp();
}	

void UHexStrategyGameInstance::UpdatePlayerDataTimeStamp()
{
	LastUpdatePlayerDataTime = FDateTime::Now();
}
