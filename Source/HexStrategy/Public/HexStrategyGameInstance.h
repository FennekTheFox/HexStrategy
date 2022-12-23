#pragma once

#include "Engine/GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "Items/ItemBase.h"

#include "HexStrategyGameInstance.generated.h"






UCLASS(BlueprintType)
class UHexStrategyGameInstance : public UGameInstance
{
	GENERATED_BODY()

//What does the game instance need to do?
//...
	////Set the game mode depending on what 
	//UFUNCTION(BlueprintCallable)
	//	void SwitchGameMode(EGameModeType NewGameMode);

	//Tells the game instance which locally loaded player data we are using
	UFUNCTION(BlueprintCallable)
		void SetUsedPlayerData(UGameFileData* NewPlayerData);

//What does the game instance need to know?
public:
	//The save file data for the local player
	UPROPERTY(BlueprintReadWrite)
		class UGameFileData* LocalPlayerData;
};