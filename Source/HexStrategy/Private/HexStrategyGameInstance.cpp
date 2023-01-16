#include "HexStrategyGameInstance.h"
#include "SaveGame/GameFileData.h"


void UHexStrategyGameInstance::SetUsedPlayerData(UGameFileData_DataAsset* NewPlayerData)
{
	LocalPlayerData = NewPlayerData;
	//LocalPlayerData->LoadData();
}
