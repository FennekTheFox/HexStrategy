#include "HexStrategyGameInstance.h"


void UHexStrategyGameInstance::SetUsedPlayerData(ULocalPlayerData* NewPlayerData)
{
	LocalPlayerData = NewPlayerData;
	LocalPlayerData->LoadData();
}

void ULocalPlayerData::LoadData()
{
	//1) Unload the current environment leve
}
