#include "Grid/PathFinders/GridPathFinder.h"

#include "Grid/GridActor.h"
#include "Libraries/UtilityLibrary.h"
#include "Grid/Components/GridMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <limits>
#include "Kismet/KismetMathLibrary.h"
#include "Libraries/GridUtilityLibrary.h"


bool FWidthSearchHelper::Step()
{

	if (OpenSet.Num() == 0)
		return false;

	UGridTile* Current = nullptr;


	OpenSet.HeapPop(Current, Comparer);
	ReachedTiles.Add(Current);


	for (auto&& KVPair : Current->ConnectedTiles)
	{
		UGridTile* UndiscoveredNeighbour = Grid->FindTileByCoordinates(KVPair.Key);

		if (ReachedTiles.Contains(UndiscoveredNeighbour)) continue;
		//If we cant pass through the tile because it is occupied, dont consider it a valid tile for a path
		if (!Agent->CanPassThroughTile(Current, UndiscoveredNeighbour)) continue;

		int NewCost = TravelCost[Current] + 1;

		if ((!TravelCost.Contains(UndiscoveredNeighbour) && NewCost <= Agent->Request.MaxCost) || (TravelCost.Contains(UndiscoveredNeighbour) && NewCost < TravelCost[UndiscoveredNeighbour]))
		{
			TravelCost.FindOrAdd(UndiscoveredNeighbour, NewCost);

			if (!OpenSet.Contains(UndiscoveredNeighbour))
			{
				OpenSet.HeapPush(UndiscoveredNeighbour, Comparer);
			}

			CameFrom.FindOrAdd(UndiscoveredNeighbour, Current);
		}
	}

	return true;
}


void FWidthSearchHelper::GetAllReachableTiles(TArray<UGridTile*>& OutTiles)
{
	OutTiles = ReachedTiles.Array();
}



bool FAStarHelper::Step()
{
	if (OpenSet.Num() == 0)
		return false;

	UGridTile* Current = nullptr;

	OpenSet.HeapPop(Current, Comparer);

	if (Current == Goal)
	{
		Success = true;
		return true;
	}

	CloseSet.Add(Current);

	for (auto&& KVPair : Current->ConnectedTiles)
	{
		UGridTile * UndiscoveredNeighbour = Grid->FindTileByCoordinates(KVPair.Key);;

		if (CloseSet.Contains(UndiscoveredNeighbour)) continue;
		//If we cant pass through the tile because it is occupied, dont consider it a valid tile for a path
		if (!Agent->CanPassThroughTile(Current, UndiscoveredNeighbour)) continue;

		int NewCost = TravelCost[Current] + DistanceHeuristic(Current, UndiscoveredNeighbour);


		if ((!TravelCost.Contains(UndiscoveredNeighbour) && NewCost <= Agent->Request.MaxCost) || (TravelCost.Contains(UndiscoveredNeighbour) && NewCost < TravelCost[UndiscoveredNeighbour]))
		{
			TravelCost.Add(UndiscoveredNeighbour, NewCost);
			FCost.Add(UndiscoveredNeighbour, NewCost + DistanceHeuristic(UndiscoveredNeighbour, Goal));


			if (!OpenSet.Contains(UndiscoveredNeighbour))
			{
				OpenSet.HeapPush(UndiscoveredNeighbour, Comparer);
			}

			CameFrom.FindOrAdd(UndiscoveredNeighbour, Current);
		}
	}

	return true;
}




void FAStarHelper::CollectPath(TArray<UGridTile*>& Result) const
{
	UGridTile* Current = Goal;

	Result.AddUnique(Current);

	while (Current != Start)
	{
		Current = CameFrom.FindChecked(Current);
		Result.Add(Current);
	}
}

int32 FAStarHelper::DistanceHeuristic(UGridTile* Probe, UGridTile* Target)
{
	//int32 MovementDistance = UGridUtilityLibrary::GetHexDistance(Probe->Coords, Target->Coords);

	return UGridUtilityLibrary::GetHexDistance_FromTiles(Probe, Target);
}








UGridPathFinderAgent::UGridPathFinderAgent()
{
}

UGridPathFinderAgent::~UGridPathFinderAgent()
{

}

UGridTile* UGridPathFinderAgent::GetStart() const
{
	return Request.Start;
}

UGridTile* UGridPathFinderAgent::GetDestination() const
{
	return Request.Goal;
}

AActor* UGridPathFinderAgent::GetSender() const
{
	return Request.Sender;
}

AGridActor* UGridPathFinderAgent::GetGrid() const
{
	return Request.GridActor;
}

const FGameplayTagContainer& UGridPathFinderAgent::GetExtraTags() const
{
	return Request.ExtraTags;
}

bool UGridPathFinderAgent::IsReachable_Implementation(const FGridPathFinderRequest InRequest)
{
	TArray<UGridTile*> trash;
	return FindPath(InRequest, trash);
}

int32 UGridPathFinderAgent::GetCost_Implementation(UGridTile* From, UGridTile* To)
{
	return UGridUtilityLibrary::GetHexDistance_FromTiles(From, To);
};


int32 UGridPathFinderAgent::Heuristic_Implementation(UGridTile* From, UGridTile* To)
{
	return GetCost(From, To);
}

bool UGridPathFinderAgent::CanPassThroughTile_Implementation(UGridTile* FromTile, UGridTile* ToTile)
{
	return true;
}

bool UGridPathFinderAgent::CanStandOnTile_Implementation(UGridTile* Tile)
{
	return true;
}

bool UGridPathFinderAgent::FindPath(const FGridPathFinderRequest InRequest, TArray<UGridTile*>& Path)
{
	Request = InRequest;

	if (!Request.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Request invalid"))
			return false;
	}
	if (!Request.GridActor)
	{
		UE_LOG(LogTemp, Log, TEXT("Grid invalid"))
			return false;
	}
	if (!CanStandOnTile_Implementation(Request.Goal))
	{
		UE_LOG(LogTemp, Log, TEXT("Goal invalid"))
			return false;
	}

	Path.Reset();
	bool Success = false;

	FAStarHelper AStarHelper(Request.Start, Request.Goal, Request.GridActor, this);

	int32 Step = 0;
	while (!Success)
	{
		if (!AStarHelper.Step())
			break;

		Success = AStarHelper.Success;

		if (++Step > Request.MaxSearchStep)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find path within MaxFindStep"));
			break;
		}
	}

	if (Success)
	{
		AStarHelper.CollectPath(Path);

		if (Request.bRemoveDest)
		{
			Path.Pop();
		}

		if (Request.MaxCost >= 0)
		{
			int32 Cost = 0;
			int32 i;

			for (i = 1; i < Path.Num(); i++)
			{
				Cost += GetCost(Path[i - 1], Path[i]);
				if (Cost > Request.MaxCost)
					break;
			}

			if (i < Path.Num())
			{
				Path.RemoveAt(i, Path.Num() - i);
				Success = false;
			}
		}
	}

	return Success;
}

void UGridPathFinderAgent::GetReachableTiles(const FGridPathFinderRequest InRequest, TArray<UGridTile*>& Result)
{
	Request = InRequest;

	Result.Reset();


	TArray<UGridTile*> OpenSet, ClosedSet;
	OpenSet.Add(Request.Start);

	FWidthSearchHelper Helper(InRequest.Start, InRequest.GridActor, this);

	//Step until theres no more reachable tiles
	while (Helper.Step());

	Helper.GetAllReachableTiles(Result);

	//for (int i = 0; i < Result.Num(); i++)
	//{
	//	TArray<AGridTile*> Neighbours;
	//	Result[i]->GetNeighbours(Neighbours);

	//	for (AGridTile* Neighbour : Neighbours)
	//	{
	//		FGridPathFinderRequest tempRequest = InRequest;
	//		tempRequest.Goal = Neighbour;

	//		if (IsReachable_Implementation(tempRequest))
	//		{
	//			Result.AddUnique(Neighbour);
	//		}
	//	}
	//}
}

FGridPathFinderRequest::FGridPathFinderRequest()
{
	bRemoveDest = false;
	MaxCost = -1;
	MaxSearchStep = 1000;
	Start = Goal = nullptr;
}

bool FGridPathFinderRequest::IsValid()
{
	return (Start && Goal);
}

//#pragma optimize( "", on )

void UGridPathFinderAgent::JumpingRequired_Implementation(UGridTile* From, UGridTile* To, bool& JumpingRequired, float& UpperBound, bool bDrawDebug)
{
	FConnectedTileData* ConnectionData = From->ConnectedTiles.Find(To->Coordinates);
	if (ConnectionData)
	{
		JumpingRequired = ConnectionData->bRequiresJump;
	}
}


//#pragma optimize( "", off )

bool UGridMovementAgent::CanPassThroughTile_Implementation(UGridTile* FromTile, UGridTile* ToTile)
{
	bool ret = true;
	//Check for occupation 
	ret = ret && (ToTile->GetOccupyingUnit() == nullptr || ToTile->GetOccupyingUnit() == Request.Sender);
	////Check for ground incline
	//ret = ret && (Tile->GetGroundNormalAngle() <= GMC->MaxWalkableSlopeIncline);

	
	return ret;
}

bool UGridMovementAgent::CanStandOnTile_Implementation(UGridTile* Tile)
{
	bool ret = true;
	//Check for occupation 
	ret = ret && (Tile->GetOccupyingUnit() == nullptr || Tile->GetOccupyingUnit() == Request.Sender);
	////Check for ground incline
	//ret = ret && (Tile->GetGroundNormalAngle() <= GMC->MaxWalkableSlopeIncline);

	return ret;
}

bool FAStarHelper::FComparer::operator()(const UGridTile& L, const UGridTile& R) const
{
	int32 LFCost = FCost->Contains(&L) ? FCost->FindChecked(&L) : TNumericLimits<int32>::Max();
	int32 RFCost = FCost->Contains(&R) ? FCost->FindChecked(&R) : TNumericLimits<int32>::Max();
	return LFCost < RFCost;
}