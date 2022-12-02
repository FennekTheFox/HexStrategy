#include "Grid/GridTile.h"

AGridTile::AGridTile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
}

