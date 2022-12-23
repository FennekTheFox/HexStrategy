#pragma once


#include "GameFramework/SaveGame.h"
#include "Units/UnitData.h"
#include "Items/ItemBase.h"

#include "GameFileData.generated.h"



UCLASS()
class UGameFileData : public USaveGame
{
	GENERATED_BODY()

public:
	//void LoadData();

public:
	//The player name
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
		FString PlayerName;
	//The player's personal unit
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
		class UUnitData* PlayerPersonalUnit;
	//The list of units at the players disposal
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
		TArray<UUnitData*> PlayerUnits;


	UPROPERTY(BlueprintReadWrite, Category = "PlayerData|Location")
		TSoftObjectPtr<UWorld> PlayerLocation_MapName;
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData|Location")
		FVector PlayerLocation_PawnLocation;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerData|Items")
		TMap<TSubclassOf<UItemBase>, int32> PlayerInventory;

	UPROPERTY(BlueprintReadWrite, Category = "Meta")
		FDateTime SaveFileCreated_DateTime;
	UPROPERTY(BlueprintReadWrite, Category = "Meta")
		FTimespan CumulativeGameTime;

};
