#pragma once

#include "GridTile.h"
#include "GridActor.generated.h"



USTRUCT(BlueprintType)
struct FTilePainterSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor OutlineColor = FLinearColor(1, 1, 1, 1);
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor FillColor = FLinearColor(1, 1, 1, 0);
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
	GENERATED_BODY()

	AGridActor();

public:
	UPROPERTY(EditAnywhere, Category = "Grid|Cooking")
		bool Bake = false;
	UPROPERTY(EditAnywhere, Category = "Grid|Cooking")
		bool BakeOnConstruction = false;
	UPROPERTY(EditAnywhere, Category = "Grid|Cooking")
		bool Debug = false;
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

	UPROPERTY(EditAnywhere, Category = "Grid|Settings|Painter")
		TSubclassOf<class UGridPainter> GridPainterClass;
	UPROPERTY(EditAnywhere, Category = "Grid|Settings|Painter")
		TMap<ETileDisplayState, FTilePainterSettings> GridPainterConfiguration;


	UPROPERTY(VisibleAnywhere, Category = "Grid|Debug")
		TMap<FIntVector, AGridTile*> GridTiles;


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
		UGridPainter* GridPainter;

	UFUNCTION(BlueprintCallable, NetMulticast, reliable)
		void SetIsActive(bool bNewActive);


public:
	UGridPainter* GetGridPainter() {return GridPainter;}
	void OnConstruction(const FTransform& Transform) override;
	void RefreshProperties();	//Makes sure all keys and subkeys are always present
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeChainProperty(PropertyChangedEvent);
		RefreshProperties();
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		RefreshProperties();
	}

	UFUNCTION(BlueprintPure)
		FVector GetCoordinateWorldCenter(int32 x, int32 y);
	UFUNCTION(BlueprintPure)
		AGridTile* GetTileClosestToCoordinates(FVector Coordinates, bool bCanBeOccupied = false);
	UFUNCTION(BlueprintPure)
		void GetSurroundingTiles(AGridTile* Origin, FInt32Interval Range, int32 MaxHeightDifference, TArray<AGridTile*>& OutTiles);

private:
	void BakeConnectedTiles(int x, int y);
	void CreateGrid();
	void CreateTilesAtCoordinates(int x, int y);
	void CleanUpGrid();
	void RegenerateGrid();
	


};