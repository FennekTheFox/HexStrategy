#pragma once

#include "Engine/GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "Items/ItemBase.h"

#include "HexStrategyGameInstance.generated.h"






UCLASS(BlueprintType)
class UHexStrategyGameInstance : public UGameInstance
{
	GENERATED_BODY()

//What does the game instance need to know?
public:
	//The save game slot we're playing on
	UPROPERTY(BlueprintReadWrite)
		FString LocalPlayerSavelot;
};