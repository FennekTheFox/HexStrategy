#pragma once

#include "GridPainter.generated.h"

class UHexGridTile;
class AGridActor;
class AGridTile;

UCLASS(DisplayName = "Default Grid Painter")
class UGridPainter : public UObject/*, FTickableGameObject*/
{
	GENERATED_BODY()


public:

	/**
	 * Updates the entire grid by iterating through every single tile
	 */
	void UpdateGrid();
	/**
	 * Function to register the grid with the grid painter
	 */
	void SetGrid(AGridActor* InGrid) { Grid = InGrid; };
	/**
	 * Function to reset the grid painter, used when SetGrid gets called (because a new grid needs to be painted.
	 */
	virtual void Clear(){};

protected:
	/**
 * Updates a specific tile, use this when the state of a small subset of tiles changes
 */
	virtual void UpdateTile(AGridTile* Tile) {};

protected:
	UPROPERTY()
		AGridActor* Grid = nullptr;
};
