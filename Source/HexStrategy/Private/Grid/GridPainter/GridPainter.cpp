#include "Grid/GridPainter/GridPainter.h"
#include "Grid/GridActor.h"

void UGridPainter::UpdateGrid()
{
	if (Grid)
	{
		for (auto&&KVPair : Grid->GridTiles)
		{
			AGridTile* Tile = KVPair.Value;
			UpdateTile(Tile);
		}
	}
}
