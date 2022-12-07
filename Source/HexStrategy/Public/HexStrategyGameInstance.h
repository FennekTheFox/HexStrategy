#pragma once

#include "Engine/GameInstance.h"
#include "GameFramework/SaveGame.h"

#include "HexStrategyGameInstance.generated.h"



UCLASS()
class ULocalPlayerData : public USaveGame
{
	GENERATED_BODY()

public: 
	void LoadData();

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


	UPROPERTY(BlueprintReadWrite, Category = "Meta")
		FDateTime SaveFileCreated_DateTime;
	UPROPERTY(BlueprintReadWrite, Category = "Meta")
		FTimespan CumulativeGameTime;
};


//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadFinished)
//
////Async blueprint node to load the data
//UCLASS()
//class ULoadPlayerData : public UBlueprintAsyncActionBase
//{
//	GENERATED_BODY()
//
//public:
//	// UBlueprintAsyncActionBase interface
//	virtual void Activate() override
//	{
//
//	}
//	//~UBlueprintAsyncActionBase interface
//
//
//	UPROPERTY()
//		ULocalPlayerData* Data;
//	UPROPERTY(BlueprintAssignable)
//		FOnLoadFinished Finished;
//};


UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	StartScreen,
	Overworld,
	LocalEncounter,
	MultiplayerEncounter
};

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
		void SetUsedPlayerData(ULocalPlayerData* NewPlayerData);

//What does the game instance need to know?
public:
	//The save file data for the local player
	UPROPERTY(BlueprintReadWrite)
		class ULocalPlayerData* LocalPlayerData;

private:
	UPROPERTY()
		EGameModeType GameModeType = EGameModeType::StartScreen;
};