#pragma once

#include "Engine/GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "Items/ItemBase.h"

#include "HexStrategyGameInstance.generated.h"






UCLASS(BlueprintType)
class UHexStrategyGameInstance : public UGameInstance
{
	GENERATED_BODY()


	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init();

	//What does the game instance need to know?
	UFUNCTION(BlueprintCallable)
		void UpdateLocalPlayerSaveSlot(const FString& newSlot);
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerDataTimeStamp();

public:
	//The save game slot we're playing on
	UPROPERTY(BlueprintReadOnly)
		FString LocalPlayerSavelot;
	/* The last time the player data was saved (or loaded)*/
	UPROPERTY(BlueprintReadOnly)
		FDateTime LastUpdatePlayerDataTime;
};