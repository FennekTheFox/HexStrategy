#include "HexStrategyGameInstance.h"
#include "GameFileData.h"


void UHexStrategyGameInstance::SetUsedPlayerData(UGameFileData* NewPlayerData)
{
	LocalPlayerData = NewPlayerData;
	//LocalPlayerData->LoadData();
}
