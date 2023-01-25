#include "GridBuilders.h"
#include "GridActor.h"
#include "Libraries/GridUtilityLibrary.h"
#include "GridTile.h"
#include "Kismet/KismetSystemLibrary.h"

bool UGridTileBuilder::Init()
{
	return Grid != nullptr;
}

uint32 UGridTileBuilder::Run()
{

	for (int y = -Grid->GridRadius; y <= Grid->GridRadius; y++)
	{

		CreateTilesAtCoordinates(private_x, y);
		if (private_x != 0)
			CreateTilesAtCoordinates(-private_x, y);
	}

	return 0;
}


void UGridTileBuilder::Exit()
{
}

void UGridTileBuilder::Stop()
{

}

void UGridTileBuilder::CreateTilesAtCoordinates(int32 x, int32 y)
{
	//UE_LOG(LogTemp, Log, TEXT("%d | %d "),x, y)
	int Distance = UGridUtilityLibrary::GetHexDistance_FromCoords(FIntVector(0, 0, 0), FIntVector(x, y, 0));
	if (!(Distance <= Grid->GridRadius)) return;

	//Line trace for all possible locations to spawn a tile
	TArray<FHitResult> Hits;
	FVector Start = Grid->GetActorLocation() + Grid->GetCoordinateWorldCenter(x, y);
	FVector End = Start;

	Start.Z = Start.Z + Grid->MaxTraceDistance / 2;
	End.Z = End.Z - Grid->MaxTraceDistance / 2;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECC_WorldStatic) };
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(Grid->GroundChannel);
	TArray<AActor*> IgnoreActors;

	//UE_LOG(LogTemp, Log, TEXT("Creating Tiles at x=%d, y=%d (%s)"),x, y, *Start.ToString())

	UKismetSystemLibrary::LineTraceMultiForObjects((UObject*)Grid->GetWorld(), Start, End, ObjectTypes, true, IgnoreActors, EDrawDebugTrace::None, Hits, true);

	float PreviousTileHeight = std::numeric_limits<float>::max();

	for (int i = 0; i < Hits.Num(); i++)
	{
		//TODO: Test for grids that'd be too close to one another here
		if ((PreviousTileHeight - Hits[i].Location.Z) < Grid->MinTileVerticalDistance)
			continue;

		FIntVector Coordinates = FIntVector(x, y, i);

		// UGridTile* newTile = Grid->CreateDefaultSubobject<UGridTile>(*FString::Printf(TEXT("Tile_%d_%d_%d"), x, y, i));
		UGridTile* newTile = NewObject<UGridTile>(Grid);

		FString PackName;
		newTile->GetPackage()->GetName(PackName);
		//newTile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d_%d"), x, y, i));
		//newTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		newTile->Coordinates = Coordinates;
		newTile->WorldLocation = Hits[i].Location;
		newTile->ParentGrid = Grid;

		OutTiles.Add(newTile);
		PreviousTileHeight = Hits[i].Location.Z;
	}
}

bool FGridNavigationBuilder::Init()
{
	return Grid != nullptr;
}

uint32 FGridNavigationBuilder::Run()
{
	for (int x = start; x < stop; x++)
	{
		Grid->BakeConnectedTiles(Grid->GridTiles[x]);
	}

	return 0;
}

void FGridNavigationBuilder::Exit()
{

}

void FGridNavigationBuilder::Stop()
{

}
