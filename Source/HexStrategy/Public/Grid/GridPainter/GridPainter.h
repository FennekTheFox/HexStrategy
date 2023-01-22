#pragma once

#include "../GridTile.h"
#include "GridPainter.generated.h"

class UGridTile;
class AGridActor;
class UGridTile;

USTRUCT(BlueprintType)
struct FTilePainterSettings
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor OutlineColor = FLinearColor(1, 1, 1, 1);
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor FillColor = FLinearColor(1, 1, 1, 0);
};



UCLASS(BlueprintType)
class UGridPainterConfig : public UDataAsset
{
	GENERATED_BODY()

	UGridPainterConfig();
	void Refresh();

public:
	UPROPERTY(EditAnywhere, Category = "Grid|Settings|Painter")
		TMap<ETileDisplayState, FTilePainterSettings> Colours;


	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	 
};

UCLASS(DisplayName = "Default Grid Painter")
class UGridPainter : public UObject/*, FTickableGameObject*/
{
	GENERATED_BODY()


public:

	/**
	 * Updates the entire grid by iterating through every single tile
	 */
	virtual void UpdateGrid();
	/**
	 * Function to register the grid with the grid painter
	 */
	void SetGrid(AGridActor* InGrid) { Grid = InGrid; };
	/**
	 * Function to reset the grid painter, used when SetGrid gets called (because a new grid needs to be painted.
	 */
	virtual void Clear() {};
	virtual void OnDestroy() {};
	/**
	* Updates a specific tile, use this when the state of a small subset of tiles changes
	*/
	virtual void UpdateTile(UGridTile* Tile) {};

protected:

protected:
	UPROPERTY()
		AGridActor* Grid = nullptr;
};
