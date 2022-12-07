#pragma once

#include "Components/SceneComponent.h"
#include "GridTile.generated.h"


class UGameplayTask;



//Enum for the different display states this tile can take
UENUM(BlueprintType)
enum class ETileDisplayState : uint8
{
	Default = 0,
	ShowMovable = 1,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ETileDisplayState, ETileDisplayState::Count);


//Interface for GameplayTasks relating to units leaving tiles
UINTERFACE()
class ULeaveTileTask : public UInterface
{
	GENERATED_BODY()
};

class ILeaveTileTask
{
	GENERATED_BODY()

public:
	//Function called at the end of execution to see if execution impeded leaving the tile
	UFUNCTION(BlueprintNativeEvent)
		bool GetMovementUnimpeded();
};



USTRUCT(BlueprintType)
struct FConnectedTileData
{
	GENERATED_BODY()

		FConnectedTileData() {}
	FConnectedTileData(int _HeightDifference, bool _bRequiresJump, int _RequiredJumpHeight, bool _bRequiresLeap, int _RequiredLeapDistance)
		: HeightDifference(_HeightDifference)
		, bRequiresJump(_bRequiresJump)
		, RequiredJumpHeight(_RequiredJumpHeight)
		, bRequiresLeap(_bRequiresLeap)
		, RequiredLeapDistance(_RequiredLeapDistance) {}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int HeightDifference = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool bRequiresJump = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int RequiredJumpHeight = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool bRequiresLeap = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int RequiredLeapDistance = 0;
};


UCLASS(BlueprintType, hideCategories = (WorldPartition, Input, Actor, Cooking, DataLayers, Replication, Rendering, Collision, HLOD, Physics, Networking))
class AGridTile : public AActor
{
	GENERATED_BODY()

		AGridTile();


public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Attributes")
		FIntVector Coordinates;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Attributes")
		class AGridActor* ParentGrid;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Attributes|Neighbours")
		TMap<AGridTile*, FConnectedTileData> ConnectedTiles;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Attributes|Neighbours")
		AGridTile* UpTile;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Attributes|Neighbours")
		AGridTile* DownTile;


public:
	UFUNCTION(BlueprintCallable)
		ETileDisplayState GetDisplayState() { return ETileDisplayState::Default; }


	//UFUNCTION(BlueprintCallable)
	bool OccupyTile(AActor* InOccupyingActor);
	//UFUNCTION(BlueprintCallable)

	//Function that asyncronously tries to leave the tile by calling all events bound to it, then calls the
	//Callback passed via parameters to signla whether or not the leave was successful
	void TryLeaveTile(AActor* InOccupyingActor, std::function<void(bool)> OnComplete);
	//Forcefully abandons the tile without triggering events
	void AbandonTile(AActor* InOccupyingActor);

	UFUNCTION(BlueprintPure)
		AActor* GetOccupyingUnit();
	UFUNCTION(BlueprintPure)
		bool GetIsOccupied();


private:
	UPROPERTY()
		AActor* OccupyingActor;
	UPROPERTY()
		TArray<UGameplayTask*> OnEnterTasks;
	UPROPERTY()
		TArray<UGameplayTask*> OnLeaveTasks;
};

