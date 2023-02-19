#pragma once

#include "GridTile.h"
#include "GridBuilders.h"
#include "GridPainter/GridPainter.h"
#include "GridActor.generated.h"

USTRUCT()
struct FTileOccupationStatus
{
	GENERATED_BODY()

	FTileOccupationStatus()
	: OccupyingActor(nullptr)
	{};

	FTileOccupationStatus(AActor* _Actor, FIntVector _Coordinates)
	: OccupyingActor(_Actor)
	, Coordinates(_Coordinates)
	{}

	UPROPERTY(VisibleAnywhere)
		AActor* OccupyingActor;
	UPROPERTY(VisibleAnywhere)
		FIntVector Coordinates;
};


UENUM(BlueprintType)
enum class EGridOrientation : uint8
{
	Vertical = 0,
	Horizontal = 1
};

UCLASS()
class AGridActor : public AActor
{
	friend class UGridTileBuilder;
	friend class FGridNavigationBuilder;

	GENERATED_BODY()

	AGridActor();

public:
	//UPROPERTY(EditAnywhere, Category = "Grid|Cooking")
	//	bool Bake = false;
	//UPROPERTY(EditAnywhere, Category = "Grid|Cooking")
	//	bool BakeOnConstruction = false;
	//UPROPERTY(EditAnywhere, Category = "Grid|Cooking")
	//	bool Debug = false;
	UPROPERTY(EditAnywhere, Category = "Grid|Cooking|TraceSettings")
		TEnumAsByte<ECollisionChannel> GroundChannel = ECC_GameTraceChannel1;
	UPROPERTY(EditAnywhere, Category = "Grid|Cooking|TraceSettings")
		TEnumAsByte<ECollisionChannel> ObstacleChannel = ECC_GameTraceChannel2;
	UPROPERTY(EditAnywhere, Category = "Grid|Cooking|TraceSettings")
		float MaxTraceDistance = 100000.0f;
	//Subdivides heights into intervals of this size
	UPROPERTY(EditAnywhere, Category = "Grid|Cooking|TraceSettings")
		float TraceOffset = 25.f;
	//Subdivides heights into intervals of this size
	UPROPERTY(EditAnywhere, Category = "Grid|Cooking|TraceSettings")
		float ProbeSize = 25.f;

	UPROPERTY(EditAnywhere, Category = "Grid|Settings")
		bool bIsActive = true;
	UPROPERTY(EditAnywhere, Category = "Grid|Settings")
		int GridRadius = 2;
	UPROPERTY(EditAnywhere, Category = "Grid|Settings")
		float TileSize = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Grid|Settings", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float TileSpacing = 0.02f;
	UPROPERTY(EditAnywhere, Category = "Grid|Settings")
		float HeightIntervals = 25.f;
	UPROPERTY(EditAnywhere, Category = "Grid|Settings")
		float MinTileVerticalDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "Grid|Settings")
		EGridOrientation GridOrientation;
		 
	UPROPERTY(EditAnywhere, Instanced, Category = "Grid|Settings|Painter")
		UGridPainter* GridPainter;
	UPROPERTY(EditAnywhere, NoClear, Category = "Grid|Settings|Painter")
		class UGridPainterConfig* GridPainterConfiguration;

	UPROPERTY()
		TArray<UGridTile*> GridTiles;
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Runtime")
		TArray<FTileOccupationStatus> Occupations;


	


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;



	//void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	UGridPainter* GetGridPainter() {return GridPainter;}
	void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintPure)
		AActor* GetActorOccupyingTile(const UGridTile* Tile);
	bool SetActorOccupyingTile(const UGridTile* Tile, AActor* Actor);
	bool UnsetActorOccupyingTile(const UGridTile* Tile);

	UFUNCTION(BlueprintPure)
		int32 GetStateIndex() {return StateIndex;}
	UFUNCTION(BlueprintPure)
		FVector GetCoordinateWorldCenter(int32 x, int32 y);
	UFUNCTION(BlueprintPure)
		UGridTile* GetTileClosestToCoordinates(FVector Coordinates, bool bCanBeOccupied = false);
	UFUNCTION(BlueprintPure)
		void GetSurroundingTiles(UGridTile* Origin, FInt32Interval Range, int32 MaxHeightDifference, TArray<UGridTile*>& OutTiles);

	UFUNCTION(BlueprintCallable)
		void SetIsActive(bool bNewActive);

	UFUNCTION(BlueprintPure)
		class UGridTile* FindTileByCoordinates(FIntVector Coordintates);


	//Editor functionality
//#if WITH_EDITORONLY_DATA
	UFUNCTION(CallInEditor, Category = "Grid|Cooking")
		void RegenerateGrid();
	UFUNCTION(CallInEditor, Category = "Grid|Cooking")
		void ClearGrid();

private:
	void BakeConnectedTiles(UGridTile* Tile);
	void CreateGrid();
	void CreateTilesAtCoordinates(int x, int y);
//#endif
	UPROPERTY(Replicated)
		int32 StateIndex = 0;
};