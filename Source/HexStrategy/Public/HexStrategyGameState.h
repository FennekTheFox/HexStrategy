#pragma once

#include <GameFramework/GameState.h>
#include "SaveGame/GameFileData.h"

#include "HexStrategyGameState.generated.h"

UCLASS(BlueprintType)
class  UMyObject : public UDataAsset
{
	GENERATED_BODY()

public:
	/*The property i want to replicate from the server to the client*/
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn), Replicated)
		FString Value;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
};


UCLASS(BlueprintType)
class AHexStrategyGameState : public AGameState
{
	GENERATED_BODY()

public:
	/*The game file data of all connected clients*/
	UPROPERTY(Replicated, BlueprintReadWrite)
		TArray<UGameFileData_DataAsset*> LoadedGameFileData;


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
};