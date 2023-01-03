#pragma once

#include "Components/SceneComponent.h"
#include "GridTile.generated.h"


class UGameplayTask;



//Enum for the different display states this tile can take
UENUM(BlueprintType)
enum class ETileDisplayState : uint8
{
	Default = 0,
	ShowAsHovered = 1,
	ShowAsSelected = 2,
	ShowAsMovable = 3,
	ShowAsAttackable = 4,
	ShowAsPath = 5,
	ShowAsInvalid = 6,
	ShowAsAttackTarget_Valid = 7,
	ShowAsAttackTarget_Invalid = 8,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ETileDisplayState, ETileDisplayState::Count);


USTRUCT()
struct FTileStateLayer
{
	GENERATED_BODY()

		void SetTileStateForLayer(const UObject* Agent, ETileDisplayState NewState);
	void ResetTileStateForLayer(const UObject* Agent);
	ETileDisplayState GetTileStateForLayer();

	TArray<TPair<const UObject*, TEnumAsByte<ETileDisplayState>>> LayerContent;
};


USTRUCT()
struct FTileState
{
	GENERATED_BODY()

		void SetTileState(const UObject* Agent, ETileDisplayState NewState, int32 Layer);
	void ResetTileState(const UObject* Agent);
	ETileDisplayState GetTileState();

public:
	UPROPERTY()
		TMap<int32, FTileStateLayer> Layers;

private:
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

	UFUNCTION(BlueprintCallable, Category = "Tile State")
		void SetTileState(const UObject* Agent, ETileDisplayState NewState, int32 Layer = 0);
	UFUNCTION(BlueprintCallable, Category = "Tile State")
		void ResetTileState(const UObject* Agent);
	UFUNCTION(BlueprintPure, Category = "Tile State")
		ETileDisplayState GetDisplayState() { return DisplayState.GetTileState(); }

	void UpdateTile();

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

	/*Notifies this tile that its about to be occupied*/
	bool PreoccupyTile(AActor* InOccupyingActor);
	/*Notifies this tile that its currently being occupoed*/
	bool OccupyTile(AActor* InOccupyingActor);

private:
	UPROPERTY()
		AActor* PreoccupyingActor;
	UPROPERTY()
		AActor* OccupyingActor;
	UPROPERTY()
		TArray<UGameplayTask*> OnEnterTasks;
	UPROPERTY()
		TArray<UGameplayTask*> OnLeaveTasks;

	UPROPERTY(VisibleAnywhere)
		FTileState DisplayState;
};

