#include "HexStrategyGameInstance.h"
#include "GameModes/GM_Overworld.h"


//void UHexStrategyGameInstance::SwitchGameMode(EGameModeType NewGameMode)
//{
//	//TSubclassOf<AGameModeBase> NewGameMode = AGameModeBase::StaticClass();
//
//	//switch (NewGameMode)
//	//{
//	//case EGameModeType::StartScreen(): break;
//	//case EGameModeType::Overworld(): NewGameMode = AGM_Overworld::StaticClass();
//	//case EGameModeType::LocalEncounter(): NewGameMode = AGM_LocalEncounter::StaticClass();
//	//case EGameModeType::MultiplayerEncounter(): break;
//	//}
//
//	//GetWorld()->SetGameMode()
//}

void UHexStrategyGameInstance::SetUsedPlayerData(ULocalPlayerData* NewPlayerData)
{
	LocalPlayerData = NewPlayerData;
	LocalPlayerData->LoadData();
}

void ULocalPlayerData::LoadData()
{
	//1) Unload the current environment leve
}
