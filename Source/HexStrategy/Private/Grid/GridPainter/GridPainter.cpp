#include "Grid/GridPainter/GridPainter.h"
#include "Grid/GridActor.h"

void UGridPainter::UpdateGrid()
{
	if (Grid)
	{
		for (auto* Tile : Grid->GridTiles)
		{
			UpdateTile(Tile);
		}
	}
}

void UGridPainterConfig::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Refresh();
}


UGridPainterConfig::UGridPainterConfig()
{
	Refresh();
}

void UGridPainterConfig::Refresh()
{
	for (ETileDisplayState State : TEnumRange<ETileDisplayState>())
	{
		Colours.FindOrAdd(State);
	}
}

