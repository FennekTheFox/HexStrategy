#include "Grid/GridActor.h"

#include "Grid/GridTile.h" 
#include "Grid/GridPainter/GridPainter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Libraries/GridUtilityLibrary.h"


AGridActor::AGridActor()
{
	//PrimaryActorTick.bCanEverTick = true;
	if (GridPainterClass == nullptr) GridPainterClass = UGridPainter::StaticClass();
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
}

void AGridActor::SetIsActive(bool bNewActive)
{
	bIsActive = bNewActive;
	GridPainter->UpdateGrid();
}

void AGridActor::OnConstruction(const FTransform& Transform)
{
	if (!GridPainter || GridPainter->GetClass() != GridPainterClass)
	{
		GridPainter = NewObject<UGridPainter>(this, GridPainterClass);
		GridPainter->SetGrid(this);
	}

	if (bBake || bBakeOnConstruction)
	{
		bBake = false;

		RegenerateGrid();
	}
}

void AGridActor::RefreshProperties()
{
	for (ETileDisplayState State : TEnumRange<ETileDisplayState>())
	{
		GridPainterConfiguration.FindOrAdd(State);
	}
}

FVector AGridActor::GetCoordinateWorldCenter(int32 x, int32 y)
{
	FVector pX, pY;
	if (GridOrientation == EGridOrientation::Horizontal)
	{
		pX = x * TileSize * FVector(.866f, 0.0f, 0.0f);
		pY = y * TileSize * FVector(.866f, 1.494f, 0.0f) / 2;
	}
	else
	{
		pY = y * TileSize * FVector(0.0f, .866f, 0.0f);
		pX = x * TileSize * FVector(1.494f, .866f, 0.0f) / 2;
	}

	return pX + pY;
}

//Constant array of all connected tiles
const  TArray<FIntVector> AdjacentTileDirections =
{
FIntVector(1, 0, 0),
FIntVector(-1, 0, 0),
FIntVector(0, 1, 0),
FIntVector(0, -1, 0),
FIntVector(-1, 1, 0),
FIntVector(1, -1, 0)
};

void AGridActor::BakeConnectedTiles(int x, int y)
{
	int z = 0;

	while (AGridTile** TilePtr = GridTiles.Find(FIntVector(x, y, z)))
	{
		AGridTile* Tile = *TilePtr;

		//Check up and down tiles
		AGridTile** UpTilePtr = GridTiles.Find(FIntVector(x, y, z+1));
		Tile->UpTile = (UpTilePtr? *UpTilePtr : nullptr);
		AGridTile** DownTilePtr = GridTiles.Find(FIntVector(x, y, z - 1));
		Tile->DownTile = (DownTilePtr ? *DownTilePtr : nullptr);

		//Check for directly adjacent tiles (-> no leap required)
		for (FIntVector dir : AdjacentTileDirections)
		{
			int h = 0;
			while (AGridTile** AdjacentTilePtr = GridTiles.Find(dir+FIntVector(x, y, h)))
			{
				AGridTile* AdjacentTile = *AdjacentTilePtr;

				//TODO: Do jump checks

				Tile->ConnectedTiles.Add(AdjacentTile);

				h++;
			}
		}

		z++;
	}
}

void AGridActor::CreateGrid()
{
	//Generates all the grid tiles
	for (int x = -GridRadius; x <= GridRadius; x++)
	{
		for (int y = -GridRadius; y <= GridRadius; y++)
		{
			int Distance = UGridUtilityLibrary::GetHexDistance_FromCoords(FIntVector(0, 0, 0), FIntVector(x, y, 0));

			//Check if the tile would be within the radius
			if (Distance <= GridRadius)
			{
				CreateTilesAtCoordinates(x, y);
			}
		}
	}

	//Maps all neighbouring tiles
	for (int x = -GridRadius; x <= GridRadius; x++)
	{
		for (int y = -GridRadius; y <= GridRadius; y++)
		{
			int Distance = UGridUtilityLibrary::GetHexDistance_FromCoords(FIntVector(0, 0, 0), FIntVector(x, y, 0));

			//Check if the tile would be within the radius
			if (Distance <= GridRadius)
			{
				BakeConnectedTiles(x, y);
			}
		}
	}
}

void AGridActor::CreateTilesAtCoordinates(int x, int y)
{
	//Line trace for all possible locations to spawn a tile
	TArray<FHitResult> Hits;
	FVector Start = GetActorLocation() + GetCoordinateWorldCenter(x, y);
	FVector End = Start;

	Start.Z = Start.Z + MaxTraceDistance / 2;
	End.Z = End.Z - MaxTraceDistance / 2;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECC_WorldStatic) };
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(TraceChannel);
	TArray<AActor*> IgnoreActors;

	//UE_LOG(LogTemp, Log, TEXT("Creating Tiles at x=%d, y=%d (%s)"),x, y, *Start.ToString())

	UKismetSystemLibrary::LineTraceMultiForObjects((UObject*)GetWorld(), Start, End, ObjectTypes, true, IgnoreActors, EDrawDebugTrace::None, Hits, true);

	for (int i = 0; i < Hits.Num(); i++)
	{
		//TODO: Test for grids that'd be too close to one another here
		FIntVector Coordinates = FIntVector(x, y, i);

		AGridTile* newTile = GetWorld()->SpawnActor<AGridTile>();
		newTile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d_%d"), x, y, i));
		newTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		newTile->Coordinates = Coordinates;
		newTile->SetActorLocation(Hits[i].Location);
		newTile->ParentGrid = this;

		GridTiles.FindOrAdd(Coordinates, newTile);
	}

}

void AGridActor::CleanUpGrid()
{
	for (auto&& KVPair : GridTiles)
	{
		AGridTile* Tile = KVPair.Value;

		if (Tile)
			Tile->Destroy();
	}

	GridTiles.Empty();
}

void AGridActor::RegenerateGrid()
{
	GridPainter->Clear();

	CleanUpGrid();
	CreateGrid();

	GridPainter->UpdateGrid();
}

