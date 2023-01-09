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
struct FTileStateLayerEntry
{
	GENERATED_BODY()

public:
	FTileStateLayerEntry()
	: Agent(nullptr)
	,State(ETileDisplayState::Default)
	{}

	FTileStateLayerEntry(const UObject* InAgent, ETileDisplayState InState)
	:Agent(InAgent)
	,State(InState)
	{}

	UPROPERTY(VisibleAnywhere)
		const UObject* Agent;
	UPROPERTY(VisibleAnywhere)
		ETileDisplayState State;

	const bool operator==(const FTileStateLayerEntry& other)
	{
		return Agent == other.Agent;
	}
};

USTRUCT()
struct FTileStateLayer
{
	GENERATED_BODY()

		void SetTileStateForLayer(const UObject* InAgent, ETileDisplayState NewState);
	void ResetTileStateForLayer(const UObject* InAgent);
	ETileDisplayState GetTileStateForLayer();

	UPROPERTY(VisibleAnywhere)
	TArray<FTileStateLayerEntry> LayerContent;
};


USTRUCT()
struct FTileState
{
	GENERATED_BODY()

		void SetTileState(const UObject* Agent, ETileDisplayState NewState, int32 Layer);
	void ResetTileState(const UObject* Agent);
	ETileDisplayState GetTileState();

public:
	UPROPERTY(VisibleAnywhere)
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

	/*Scene component so tile actors dont get unloaded by world partitioning, TODO find a better way to do this*/
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* SMC;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Attributes|Neighbours")
		TMap<AGridTile*, FConnectedTileData> ConnectedTiles;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Attributes|Neighbours")
		AGridTile* UpTile;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Attributes|Neighbours")
		AGridTile* DownTile;
	/*The current display state of the Tile, does not replicate*/
	UPROPERTY(VisibleAnywhere)
		FTileState DisplayState;

	/*The actor that occupies the tile. Replicated because movement is calculated locally*/
	UPROPERTY(Replicated, VisibleAnywhere)
		AActor* OccupyingActor;


	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

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
	/*The unit that announces its attachment to this tile, so that no other unit tries to attach before 
	the pre-occupant arrives and occupies properly*/
	UPROPERTY()
		AActor* PreoccupyingActor;
	/*Tasks bound to the tile, executing once a unit enters it.*/
	UPROPERTY()
		TArray<UGameplayTask*> OnEnterTasks;
	/*Tasks bound to the tile, executing once a unit leaves it.*/
	UPROPERTY()
		TArray<UGameplayTask*> OnLeaveTasks;
};

