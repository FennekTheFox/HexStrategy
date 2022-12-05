#include "Grid/GridTile.h"

AGridTile::AGridTile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
}

bool AGridTile::OccupyTile(AActor* InOccupyingActor)
{
	if (!OccupyingActor)
	{
		OccupyingActor = InOccupyingActor;
		return true;
	}

	return false;
}

bool AGridTile::TryLeaveTile(AActor* InOccupyingActor)
{
	if (OccupyingActor && OccupyingActor == InOccupyingActor)
	{
		OccupyingActor = nullptr;
		return true;
	}

	return false;
}

AActor* AGridTile::GetOccupyingUnit()
{
	return OccupyingActor;
}

bool AGridTile::GetIsOccupied()
{
	return (OccupyingActor != nullptr);
}

