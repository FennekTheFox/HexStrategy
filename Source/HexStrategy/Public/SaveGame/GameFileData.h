#pragma once


#include "GameFramework/SaveGame.h"
#include "Units/UnitData.h"
#include "Items/ItemBase.h"
#include "Items/ItemManagement.h"
#include "ObjectRecord.h"

#include "GameFileData.generated.h"

class UUnitData;


/*Contains data that describes the save game, not the actual game state itself.*/
USTRUCT(BlueprintType)
struct FGameFileMetaData
{
	GENERATED_BODY();


	//The player name
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
		FString PlayerName;


	UPROPERTY(BlueprintReadWrite, Category = "Meta")
		FDateTime SaveFileCreated_DateTime;
	UPROPERTY(BlueprintReadWrite, Category = "Meta")
		FTimespan CumulativeGameTime;
};

/*The serializable save game object*/
UCLASS(BlueprintType)
class UGameFileRecord : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
		FGameFileMetaData MetaData;

	UPROPERTY(SaveGame)
		FObjectRecord PlayerUnitCharacter;
	UPROPERTY(SaveGame)
		TArray<FObjectRecord> PlayerUnits;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerData|Location")
		FName PlayerLocation_MapName;
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData|Location")
		FVector PlayerLocation_PawnLocation;



	UPROPERTY(SaveGame)
		TMap<FName, FItemManagementData> PlayerInventory;
};


/*The runtime object that describes the game state*/
UCLASS(BlueprintType)
class UGameFileData : public UObject
{
	GENERATED_BODY()

public:
	/*Meta Data describing the save game object*/
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
		FGameFileMetaData MetaData;
	//The player's personal unit
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
		UUnitData* PlayerUnitCharacter;
	//The list of units at the players disposal
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
		TArray<UUnitData*> PlayerUnits;


	UPROPERTY(BlueprintReadWrite, Category = "PlayerData|Location")
		FName PlayerLocation_MapName;
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData|Location")
		FVector PlayerLocation_PawnLocation;

	/*The players inventory, denoting each item in his posession, how many he has and which unit holds them*/
	UPROPERTY(BlueprintReadWrite, Category = "PlayerData|Items")
		TMap<FName, FItemManagementData> PlayerInventory;



public:
	UFUNCTION(BlueprintCallable, Category = "GameFileData")
		void AddItemToPlayerInventory(class UItemBase* ItemToAdd, int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "GameFileData")
		bool RemoveItemFromPlayerInventory(class UItemBase* ItemToAdd, int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "GameFileData")
		bool DoesPlayerHaveItem(class UItemBase* ItemToAdd, int32 Amount);


	/*Turns the data object into a serializable save game object*/
	UFUNCTION(BlueprintPure, Category = "GameFileData|Serialization")
		UGameFileRecord* SerializeToRecord();
	/*Creates a Game data object from a serialized record*/
	UFUNCTION(BlueprintPure, Category = "GameFileData|Serialization")
		static UGameFileData* DeserializeFromRecord(UGameFileRecord* Record);
};
