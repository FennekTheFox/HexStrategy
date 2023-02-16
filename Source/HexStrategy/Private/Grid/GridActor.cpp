#include "Grid/GridActor.h"

#include "Grid/GridTile.h" 
#include "Grid/GridPainter/GridPainter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Libraries/GridUtilityLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Net/DataBunch.h"
#include "Grid/GridBuilders.h"
#include "../Plugins/Editor/EditorScriptingUtilities/Source/EditorScriptingUtilities/Public/EditorAssetLibrary.h"


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
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));

	bReplicates = true;

	for (UGridTile* Tile : GridTiles)
	{

	}
}


AActor* AGridActor::GetActorOccupyingTile(const UGridTile* Tile)
{
	if(!ensure(Tile)) return nullptr;
	
	FIntVector Coordinates  = Tile->Coordinates;

	FTileOccupationStatus* s = Occupations.FindByPredicate([Coordinates](const FTileOccupationStatus& state) {
			return state.Coordinates == Coordinates;
		});

	if(s) return s->OccupyingActor;

	return nullptr;
}

bool AGridActor::SetActorOccupyingTile(const UGridTile* Tile, AActor* Actor)
{
	if (!ensure(Tile && Actor)) return false;

	FIntVector Coordinates = Tile->Coordinates;

	int32 s = Occupations.IndexOfByPredicate([Coordinates](const FTileOccupationStatus& state) {
		return state.Coordinates == Coordinates;
		});

	if (s != INDEX_NONE) return false;

	Occupations.Add(FTileOccupationStatus(Actor, Coordinates));
	return true;
}

bool AGridActor::UnsetActorOccupyingTile(const UGridTile* Tile)
{
	if (!ensure(Tile)) return false;

	FIntVector Coordinates = Tile->Coordinates;

	int32 s = Occupations.IndexOfByPredicate([Coordinates](const FTileOccupationStatus& state) {
		return state.Coordinates == Coordinates;
		});

	if(!ensure(s != INDEX_NONE)) return false;

	Occupations.RemoveAt(s);
	return true;
}

void AGridActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGridActor, Occupations);
}

bool AGridActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	bWroteSomething |= Channel->ReplicateSubobjectList(GridTiles, *Bunch, *RepFlags);

	return bWroteSomething;
}

//void AGridActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent.GetPropertyName == TEXT("GridPainter"))
//	{
//		PropertyChangedEvent.
//	};
//
//	if (PropertyChangedEvent.)
//}

void AGridActor::SetIsActive(bool bNewActive)
{
	bIsActive = bNewActive;

	if(GridPainter)
	{
		GridPainter->SetGrid(this);
		GridPainter->UpdateGrid();
	}
}

class UGridTile* AGridActor::FindTileByCoordinates(FIntVector InCoordintates)
{
	for (UGridTile* Tile : GridTiles)
	{
		if (Tile->Coordinates == InCoordintates)
			return Tile;
	}

	return nullptr;
}


void AGridActor::OnConstruction(const FTransform& Transform)
{
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

UGridTile* AGridActor::GetTileClosestToCoordinates(FVector Coordinates, bool bCanBeOccupied)
{
	FVector Offset = Coordinates - GetActorLocation();
	UGridTile* ret = nullptr;

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

	while (UGridTile* comp = FindTileByCoordinates(FIntVector(closestX, closestY, h)))
	{
		float compDist = (comp->WorldLocation - Coordinates).Size();

		if (bCanBeOccupied || !comp->GetIsOccupied())
		{
			if (compDist < minDist)
			{
				minDist = compDist;
				ret = comp;
			}
		}

		h++;
	}

	if (ret)
		return ret;

	//If we dont find one with the closest xy coordinates, persue a brute force 
	//TODO: find a better way to do this
	for (auto* comp : GridTiles)
	{
		float compDist = (comp->WorldLocation - Coordinates).Size();

		if (bCanBeOccupied || !comp->GetIsOccupied())
		{
			if (compDist < minDist)
			{
				minDist = compDist;
				ret = comp;
			}
		}
	}

	return ret;
}


void AGridActor::GetSurroundingTiles(UGridTile* Origin, FInt32Interval Range, int32 MaxHeightDifference, TArray<UGridTile*>& OutTiles)
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
			for (int h = 0; UGridTile * Tile = FindTileByCoordinates(FIntVector(Current_Coords.X, Current_Coords.Y, h)); h++)
			{
				OutTiles.Add(Tile);
			}
		}
	}
}

//#if WITH_EDITORONLY_DATA

void AGridActor::RegenerateGrid()
{
	if (GridPainter)
	{
		GridPainter->SetGrid(this);
		GridPainter->Clear();
	}
	ClearGrid();
	CreateGrid();

	if (GridPainter) GridPainter->UpdateGrid();

	UEditorAssetLibrary::SaveAsset(this->GetPathName());
}

void AGridActor::BakeConnectedTiles(UGridTile* Tile)
{
	int x = Tile->Coordinates.X;
	int y = Tile->Coordinates.Y;
	int z = Tile->Coordinates.Z;

	EDrawDebugTrace::Type DrawDebugType = (false ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None);
	TArray<AActor*> IgnoreActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleTypes = { UEngineTypes::ConvertToObjectType(ObstacleChannel) };

	//Check up and down tiles
	UGridTile* UpTilePtr = FindTileByCoordinates(FIntVector(x, y, z + 1));
	UGridTile* DownTilePtr = FindTileByCoordinates(FIntVector(x, y, z - 1));

	//Check for directly adjacent tiles (-> no leap required)
	for (FIntVector dir : AdjacentTileDirections)
	{
		int h = 0;
		while (UGridTile* AdjacentTile = FindTileByCoordinates(dir + FIntVector(x, y, h)))
		{
			//1) Check if the path between the two adjacent tiles is walkable
			//Traces a sphere path above the tiles to check if theres any collision
			FHitResult Hit;
			FVector DirectStart = Tile->WorldLocation + FVector::UpVector * (ProbeSize + TraceOffset);
			FVector DirectEnd = AdjacentTile->WorldLocation + FVector::UpVector * (ProbeSize + TraceOffset);


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
					FConnectedTileData& ConnectionData = Tile->ConnectedTiles.Add(AdjacentTile->Coordinates);
					ConnectionData.HeightDifference = (DirectEnd.Z - DirectStart.Z) / HeightIntervals;
					ConnectionData.bRequiresJump = true;
					ConnectionData.RequiredJumpHeight = 1 + (Z_Peak - FMath::Min(DirectStart.Z, DirectEnd.Z)) / HeightIntervals;
				}
				//Line trace between the two peak points

			}
			else
			{
				FConnectedTileData& ConnectionData = Tile->ConnectedTiles.Add(AdjacentTile->Coordinates);
				ConnectionData.HeightDifference = (DirectEnd.Z - DirectStart.Z) / HeightIntervals;
			}

			h++;
		}
	}
}

void AGridActor::CreateGrid()
{
	TArray<UGridTileBuilder> TileBuilders;
	TArray<FGridNavigationBuilder> NavBuilders;

	GridTiles.Empty();

	//Estimate how many threads can be used for generation
	int32 NumTreads = GridRadius + 1; //FMath::Max(1, FMath::Min(FGenericPlatformMisc::NumberOfCoresIncludingHyperthreads() - 2, GridRadius));
	float IntervalSize = (float)GridRadius / NumTreads;


	TileBuilders.Reserve(NumTreads);

	UE_LOG(LogTemp, Log, TEXT("Building Grid with %d threads."), NumTreads);

	//Initiate the threads for creating the tiles
	TArray<FRunnableThread*> BuilderThreads;
	for (int n = 0; n < NumTreads; n++)
	{
		TileBuilders.Add(UGridTileBuilder(this, n));
		FRunnableThread* newThread = FRunnableThread::Create(&TileBuilders[n], TEXT("TileBuilderThread"));
		BuilderThreads.Add(newThread);
	}

	//Run the threads
	for (int i = 0; i < BuilderThreads.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Waiting for %d/%d"), i, BuilderThreads.Num())

		BuilderThreads[i]->WaitForCompletion();
		GridTiles.Append(TileBuilders[i].OutTiles);
	}


	//UE_LOG(LogTemp, Log, TEXT("done"))

	//NumTreads = FMath::Max(1, FMath::Min(FGenericPlatformMisc::NumberOfCoresIncludingHyperthreads() - 2, GridRadius));
	
	ParallelFor(GridTiles.Num(), [&](int32 index)
		{
			BakeConnectedTiles(GridTiles[index]);
		});

	//BuilderThreads.Empty();
	//IntervalSize = GridTiles.Num()/ NumTreads;
	////Maps all neighbouring tiles
	//for (int n = 0; n < NumTreads; n++)
	//{
	//	int32 start = n * IntervalSize;
	//	int32 stop = (n + 1) * IntervalSize;
	//	NavBuilders.Add(FGridNavigationBuilder(this, start, stop));
	//	FRunnableThread* newThread = FRunnableThread::Create(&NavBuilders[n], TEXT("TileNavbuilderThread"));
	//	BuilderThreads.Add(newThread);
	//}

	////Run the threads
	//for (int i = 0; i < BuilderThreads.Num(); i++)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Waiting for %d/%d"), i, BuilderThreads.Num())

	//		BuilderThreads[i]->WaitForCompletion();
	//}
}

void AGridActor::CreateTilesAtCoordinates(int x, int y)
{
	int Distance = UGridUtilityLibrary::GetHexDistance_FromCoords(FIntVector(0, 0, 0), FIntVector(x, y, 0));
	if (!(Distance <= GridRadius)) return;

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

		UGridTile* newTile = NewObject<UGridTile>(this);

		FString PackName;
		newTile->GetPackage()->GetName(PackName);
		//newTile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d_%d"), x, y, i));
		//newTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		newTile->Coordinates = Coordinates;
		newTile->WorldLocation = Hits[i].Location;
		newTile->ParentGrid = this;

		GridTiles.Add(newTile);
		PreviousTileHeight = Hits[i].Location.Z;
	}
}

void AGridActor::ClearGrid()
{
	for (UGridTile* Tile : GridTiles)
	{
		//if (Tile)
		//	Tile->Destroy();
	}

	GridTiles.Empty();
}

//#endif

