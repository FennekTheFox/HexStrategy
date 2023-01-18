#pragma once


#include "GameFramework/SaveGame.h"
#include "GameFramework/PlayerState.h"
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
		FString PlayerName;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meta")
		FDateTime SaveFileCreated_DateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meta")
		FDateTime LastSave_DateTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meta")
		FTimespan CumulativeGameTime;
};

/*The serializable save game object. Used for communication between client and server whenever
Game files are loaded (then we pass the server our local game file data) or saved (then we request
this information from the server)*/
USTRUCT(BlueprintType)
struct FGameFileRecord
{
	GENERATED_BODY()
public:
	UPROPERTY(SaveGame, EditAnywhere)
		FGameFileMetaData MetaData;

	UPROPERTY(SaveGame, EditAnywhere)
		FObjectRecord PlayerUnitCharacter;
	UPROPERTY(SaveGame)
		TArray<FObjectRecord> PlayerUnits;


	UPROPERTY(SaveGame, EditAnywhere)
		FGameplayTagContainer GameplayFlags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData|Location")
		FName PlayerLocation_MapName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData|Location")
		FVector PlayerLocation_PawnLocation;

	UPROPERTY(SaveGame, EditAnywhere)
		TArray<FItemManagementData> PlayerInventory;
};

/*A USaveGame wrapper for the client side representation of the player's game file data*/
UCLASS(BlueprintType)
class UGameFile_SaveGameWrapper : public USaveGame
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly, SaveGame)
		FGameFileRecord InternalRecord;
};


/*The info actor (for replication purposes) that contains the player data*/
UCLASS(BlueprintType)
class APlayerGameFileData : public APlayerState
{
	GENERATED_BODY()

	APlayerGameFileData();

public:
	/*Meta Data describing the save game object*/
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
		FGameFileMetaData MetaData;
	//The player's personal unit
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
		UUnitData* PlayerUnitCharacter;
	//The list of units at the players disposal
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
		TArray<UUnitData*> PlayerUnits;


	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "PlayerData|Location")
		FName PlayerLocation_MapName;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "PlayerData|Location")
		FVector PlayerLocation_PawnLocation;

	/*The players inventory, denoting each item in his posession, how many he has and which unit holds them*/
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "PlayerData|Items")
		TArray<FItemManagementData> PlayerInventory;
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere, Category = "PlayerData|Flags")
		FGameplayTagContainer GameplayFlags;


	bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:
	/*Requests adding an item to the inventory on the server side*/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GameFileData")
		void AddItemToPlayerInventory(class UItemBase* ItemToAdd, int32 Amount);
	/*Requests removing an item from the inventory on the server side*/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GameFileData")
		void RemoveItemFromPlayerInventory(class UItemBase* ItemToAdd, int32 Amount);


	UFUNCTION(BlueprintCallable, Category = "GameFileData")
		bool DoesPlayerHaveItem(class UItemBase* ItemToAdd, int32 Amount);

	/*Tries to add a new flag to the list of flags associated with the game file
	@param NewFlag - the flag to add
	@return - true if the flag could be added, false if it already existed*/
	UFUNCTION(Server, reliable, BlueprintCallable, Category = "GameFileData")
		void AddFlag(FGameplayTag NewFlag);
	/*
	*Checks if the flag is registered with the game file
	*/
	UFUNCTION(BlueprintCallable, Category = "GameFileData")
		bool HasFlag(FGameplayTag Flag);



	/*Turns the data object into a serializable save game object*/
	UFUNCTION(BlueprintPure, Category = "GameFileData|Serialization")
		UGameFile_SaveGameWrapper* SerializeToWrapper();
	/*Creates a Game data object from a serialized record*/
	UFUNCTION(BlueprintCallable, Category = "GameFileData|Serialization")
		void InitializeFromRecord(UPARAM(ref)const FGameFileRecord& Record);
	UFUNCTION(BlueprintPure, Category = "GameFileData|Serialization")
		void GetRecord(FGameFileRecord& OutRecord);
protected:
	void BeginPlay() override;

};




/*A data asset for testing purposes.*/
UCLASS(BlueprintType)
class UGameFileData_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/*Meta Data describing the save game object*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData", meta=(ExposeOnSpawn))
		FGameFileMetaData MetaData;
	//The player's personal unit
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData", meta = (ExposeOnSpawn))
		UUnitData* PlayerUnitCharacter;
	//The list of units at the players disposal
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
		TArray<UUnitData*> PlayerUnits;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData|Location")
		FName PlayerLocation_MapName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData|Location")
		FVector PlayerLocation_PawnLocation;

	/*The players inventory, denoting each item in his posession, how many he has and which unit holds them*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData|Items")
		TArray<FItemManagementData> PlayerInventory;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PlayerData|Flags")
		FGameplayTagContainer GameplayFlags;	

public:
	/*Turns the data object into a serializable save game object*/
	UFUNCTION(BlueprintPure, Category = "GameFileData|Serialization")
		UGameFile_SaveGameWrapper* SerializeToWrapper();
	UFUNCTION(BlueprintPure, Category = "GameFileData|Serialization")
		static void GetRecord(UGameFileData_DataAsset* Data, FGameFileRecord& OutRecord);
	/*Creates a Game data object from a serialized record*/
	UFUNCTION(BlueprintPure, Category = "GameFileData|Serialization")
		static UGameFileData_DataAsset* ExtractFromWrapper(UGameFile_SaveGameWrapper* Wrapper, UObject* Outer);
	/*Creates a Game data object from a serialized record*/
	UFUNCTION(BlueprintPure, Category = "GameFileData|Serialization")
		static UGameFileData_DataAsset* FromRecord(UPARAM(ref)const FGameFileRecord& Record, UObject* Outer);
};
