#include "Grid/GridActor.h"

#include "Grid/GridTile.h" 
#include "Grid/GridPainter/GridPainter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Libraries/GridUtilityLibrary.h"


#define SQRT_THREE_HALVED 0.866f

static const  TArray<FIntVector> AdjacentTileDirections =
{
FIntVector(1, 0, 0),
FIntVector(-1, 0, 0),
FIntVector(0, 1, 0),
FIntVector(0, -1, 0),
FIntVector(-1, 1, 0),
FIntVector(1, -1, 0)
};


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

	if (Bake || BakeOnConstruction)
	{
		Bake = false;

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
	if (GridOrientation == EGridOrientation::Vertical)
	{
		pX = x * TileSize * FVector(SQRT_THREE_HALVED, 0.0f, 0.0f);
		pY = y * TileSize * FVector(SQRT_THREE_HALVED, 1.5f, 0.0f) / 2;
	}
	else
	{
		pY = y * TileSize * FVector(0.0f, SQRT_THREE_HALVED, 0.0f);
		pX = x * TileSize * FVector(1.5f, SQRT_THREE_HALVED, 0.0f) / 2;
	}

	return pX + pY;
}

//Constant array of all connected tiles

AGridTile* AGridActor::GetTileClosestToCoordinates(FVector Coordinates, bool bCanBeOccupied)
{
	FVector Offset = Coordinates - GetActorLocation();
	AGridTile* ret = nullptr;

	int32 closestY, closestX;

	//Find closest XY coordinates
	if (GridOrientation == EGridOrientation::Vertical)
	{
		//In a horizontally aligned grid

		closestY = FMath::RoundToInt(Offset.Y / (0.75f * TileSize));
		closestX = FMath::RoundToInt((Offset.X - (SQRT_THREE_HALVED / 2 * closestY * TileSize)) / (SQRT_THREE_HALVED * TileSize));
	}
	else
	{
		//In a vertically grid

		closestX = FMath::RoundToInt(Offset.X / (0.75f * TileSize));
		closestY = FMath::RoundToInt((Offset.Y - (SQRT_THREE_HALVED / 2 * closestX * TileSize)) / (SQRT_THREE_HALVED * TileSize));
	}

	//Iterate vertically
	int32 h = 0;
	float minDist = std::numeric_limits<float>::max();

	while (AGridTile** compptr = GridTiles.Find(FIntVector(closestX, closestY, h)))
	{
		AGridTile* comp = *compptr;
		float compDist = (comp->GetActorLocation() - Coordinates).Size();

		if (compDist < minDist)
		{
			minDist = compDist;
			ret = comp;
		}

		h++;
	}

	if (ret)
		return ret;

	//If we dont find one with the closest xy coordinates, persue a brute force 
	//TODO: find a better way to do this
	for (auto&& KVPair : GridTiles)
	{
		AGridTile* comp = KVPair.Value;

		float compDist = (comp->GetActorLocation() - Coordinates).Size();

		if (compDist < minDist)
		{
			minDist = compDist;
			ret = comp;
		}
	}

	return ret;
}


void AGridActor::GetSurroundingTiles(AGridTile* Origin, FInt32Interval Range, int32 MaxHeightDifference, TArray<AGridTile*>& OutTiles)
{
	if (ensure(Origin))
	{
		OutTiles.Reset();

		TArray<FIntVector> OpenCoords;

		FIntVector OriginCoords = Origin->Coordinates;

		//Seed all tiles with the fitting distance range as open
		for (int x = -1 * Range.Max; x <= Range.Max; x++)
		{
			for (int y = -Range.Max; y <= Range.Max; y++)
			{
				FIntVector SeedCoords(OriginCoords.X + x, OriginCoords.Y + y, 0);
				int32 dist = UGridUtilityLibrary::GetHexDistance_FromCoords(OriginCoords, SeedCoords);

				if (dist <= Range.Max && dist >= Range.Min)
				{
					OpenCoords.Add(SeedCoords);
				}
			}
		}

		//While we got open tiles to process
		for (auto It = OpenCoords.CreateIterator(); It; It++)
		{
			FIntVector Current_Coords = *It;

			//Discover all tiles;
			//Iterate over layers of the current coordinates;
			for (int h = 0; AGridTile** TilePtr = GridTiles.Find(FIntVector(Current_Coords.X, Current_Coords.Y, h)); h++)
			{
				AGridTile* pvt = *TilePtr;
				OutTiles.Add(pvt);
			}
		}
	}
}

void AGridActor::BakeConnectedTiles(int x, int y)
{
	int z = 0;

	while (AGridTile** TilePtr = GridTiles.Find(FIntVector(x, y, z)))
	{
		EDrawDebugTrace::Type DrawDebugType = (Debug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None);
		TArray<AActor*> IgnoreActors;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleTypes = { UEngineTypes::ConvertToObjectType(ObstacleChannel) };

		AGridTile* Tile = *TilePtr;

		//Check up and down tiles
		AGridTile** UpTilePtr = GridTiles.Find(FIntVector(x, y, z + 1));
		Tile->UpTile = (UpTilePtr ? *UpTilePtr : nullptr);
		AGridTile** DownTilePtr = GridTiles.Find(FIntVector(x, y, z - 1));
		Tile->DownTile = (DownTilePtr ? *DownTilePtr : nullptr);

		//Check for directly adjacent tiles (-> no leap required)
		for (FIntVector dir : AdjacentTileDirections)
		{
			int h = 0;
			while (AGridTile** AdjacentTilePtr = GridTiles.Find(dir + FIntVector(x, y, h)))
			{
				AGridTile* AdjacentTile = *AdjacentTilePtr;

				//1) Check if the path between the two adjacent tiles is walkable
				//Traces a sphere path above the tiles to check if theres any collision
				FHitResult Hit;
				FVector DirectStart = Tile->GetActorLocation() + FVector::UpVector * (ProbeSize + TraceOffset);
				FVector DirectEnd = AdjacentTile->GetActorLocation() + FVector::UpVector * (ProbeSize + TraceOffset);


				if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, DirectStart, DirectEnd, ProbeSize, ObstacleTypes, true, IgnoreActors, DrawDebugType, Hit, true, FLinearColor::Green, FLinearColor::Red, 5.0f))
				{
					//Check if a jump is required
					//First check if a direct jump is possible
					// -> Approximate the jumping arc
					float Z_Peak = FMath::Max(DirectStart.Z, DirectEnd.Z) + TraceOffset;

					//Line trace up above both tiles up to the max of their Z + Offset
					FVector JumpPeak1 = FVector(DirectStart.X, DirectStart.Y, Z_Peak);
					FVector JumpPeak2 = FVector(DirectEnd.X, DirectEnd.Y, Z_Peak);

					bool bCanJump = false;

					bool Obstucted1 = UKismetSystemLibrary::SphereTraceSingleForObjects(this, DirectStart, JumpPeak1, ProbeSize, ObstacleTypes, true, IgnoreActors, DrawDebugType, Hit, true, FLinearColor::Yellow, FLinearColor::Red, 5.0f);

					if (!Obstucted1)
					{
						bool Obstucted2 = UKismetSystemLibrary::SphereTraceSingleForObjects(this, DirectEnd, JumpPeak2, ProbeSize, ObstacleTypes, true, IgnoreActors, DrawDebugType, Hit, true, FLinearColor::Yellow, FLinearColor::Red, 5.0f);

						if (!Obstucted2)
						{
							bool Obstucted3 = UKismetSystemLibrary::SphereTraceSingleForObjects(this, JumpPeak1, JumpPeak2, ProbeSize, ObstacleTypes, true, IgnoreActors, DrawDebugType, Hit, true, FLinearColor::Yellow, FLinearColor::Red, 5.0f);

							if (!Obstucted3)
							{
								bCanJump = true;
							}
						}
					}

					if (bCanJump)
					{
						FConnectedTileData& ConnectionData = Tile->ConnectedTiles.Add(AdjacentTile);
						ConnectionData.HeightDifference = (DirectEnd.Z - DirectStart.Z) / HeightIntervals;
						ConnectionData.bRequiresJump = true;
						ConnectionData.RequiredJumpHeight = 1 + (Z_Peak - FMath::Min(DirectStart.Z, DirectEnd.Z)) / HeightIntervals;
					}
					//Line trace between the two peak points

				}
				else
				{
					FConnectedTileData& ConnectionData = Tile->ConnectedTiles.Add(AdjacentTile);
					ConnectionData.HeightDifference = (DirectEnd.Z - DirectStart.Z) / HeightIntervals;
				}

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
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(GroundChannel);
	TArray<AActor*> IgnoreActors;

	//UE_LOG(LogTemp, Log, TEXT("Creating Tiles at x=%d, y=%d (%s)"),x, y, *Start.ToString())

	UKismetSystemLibrary::LineTraceMultiForObjects((UObject*)GetWorld(), Start, End, ObjectTypes, true, IgnoreActors, EDrawDebugTrace::None, Hits, true);

	float PreviousTileHeight = std::numeric_limits<float>::max();

	for (int i = 0; i < Hits.Num(); i++)
	{
		//TODO: Test for grids that'd be too close to one another here
		if ((PreviousTileHeight - Hits[i].Location.Z) < MinTileVerticalDistance)
			continue;

		FIntVector Coordinates = FIntVector(x, y, i);

		AGridTile* newTile = GetWorld()->SpawnActor<AGridTile>();
		newTile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d_%d"), x, y, i));
		newTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		newTile->Coordinates = Coordinates;
		newTile->SetActorLocation(Hits[i].Location);
		newTile->ParentGrid = this;

		GridTiles.FindOrAdd(Coordinates, newTile);
		PreviousTileHeight = Hits[i].Location.Z;
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

