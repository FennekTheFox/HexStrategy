#include "HexStrategyGameInstance.h"
#include "SaveGame/GameFileData.h"


void UHexStrategyGameInstance::SetUsedPlayerData(UGameFileData* NewPlayerData)
{
	LocalPlayerData = NewPlayerData;
	//LocalPlayerData->LoadData();
}
