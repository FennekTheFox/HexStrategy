#pragma once

#include "GameplayTagContainer.h"
#include "GridPathFinder.generated.h"

class UGridPathFinderAgent;


struct FWidthSearchHelper
{
public:
	FWidthSearchHelper(UGridTile* Origin, AGridActor* InGrid, UGridPathFinderAgent* InAgent)
		:Start(Origin)
		, Grid(InGrid)
		, Agent(InAgent)
	{
		Comparer.TravelCost = &TravelCost;
		TravelCost.FindOrAdd(Start, 0.f);
		OpenSet.HeapPush(Start, Comparer);
	};

	bool Step();
	void GetAllReachableTiles(TArray<UGridTile*>& OutTiles);
public:
	bool Success = false;
private:
	struct FComparer
	{
		bool operator()(const UGridTile& L, const UGridTile& R) const
		{
			int32 LTravelCost = TravelCost->Contains(&L) ? TravelCost->FindChecked(&L) : TNumericLimits<int32>::Max();
			int32 RTravelCost = TravelCost->Contains(&R) ? TravelCost->FindChecked(&R) : TNumericLimits<int32>::Max();
			return LTravelCost <= RTravelCost;
		}
		TMap<UGridTile*, int32>* TravelCost;
	} Comparer;

	TArray<UGridTile*> OpenSet;
	TSet<UGridTile*> ReachedTiles;
	TMap<UGridTile*, int32> TravelCost;
	TMap<UGridTile*, UGridTile*> CameFrom;
	UGridTile* Start;
	AGridActor* Grid;
	UGridPathFinderAgent* Agent;
};

struct FAStarHelper
{
public:
	FAStarHelper(UGridTile* From, UGridTile* To, AGridActor* InGrid, UGridPathFinderAgent* InAgent)
		:Start(From)
		, Goal(To)
		, Grid(InGrid)
		, Agent(InAgent)
	{
		Comparer.FCost = &FCost;

		TravelCost.FindOrAdd(Start, 0.f);
		FCost.FindOrAdd(Start, 100 * DistanceHeuristic(Start, Goal));

		OpenSet.HeapPush(Start, Comparer);
	};

	bool Step();
	void CollectPath(TArray<UGridTile*>& Result) const;
public:
	bool Success = false;
private:
	int32 DistanceHeuristic(UGridTile* Probe, UGridTile* Target);

	struct FComparer
	{
		bool operator()(const UGridTile& L, const UGridTile& R) const;
		TMap<UGridTile*, int32>* FCost;
	} Comparer;

	TArray<UGridTile*> OpenSet;
	TSet<UGridTile*> CloseSet;
	TMap<UGridTile*, int32> FCost;
	TMap<UGridTile*, int32> TravelCost;
	TMap<UGridTile*, UGridTile*> CameFrom;
	UGridTile* Start;
	UGridTile* Goal;
	AGridActor* Grid;
	UGridPathFinderAgent* Agent;
};


class AGridActor;
class UGridTile;

USTRUCT(BlueprintType)
struct FGridPathFinderRequest
{
	GENERATED_BODY()

public:
	FGridPathFinderRequest();

	UPROPERTY(BlueprintReadWrite, Category = "HexGridPathFindingRequest")
		AActor* Sender;

	UPROPERTY(BlueprintReadWrite, Category = "HexGridPathFindingRequest")
		AGridActor* GridActor;

	UPROPERTY(BlueprintReadWrite, Category = "HexGridPathFindingRequest")
		UGridTile* Start;

	UPROPERTY(BlueprintReadWrite, Category = "HexGridPathFindingRequest")
		UGridTile* Goal;

	UPROPERTY(BlueprintReadWrite, AdvancedDisplay, Category = "HexGridPathFindingRequest")
		int32 MaxCost;

	UPROPERTY(BlueprintReadWrite, AdvancedDisplay, Category = "HexGridPathFindingRequest")
		int32 MaxSearchStep;

	UPROPERTY(BlueprintReadWrite, AdvancedDisplay, Category = "HexGridPathFindingRequest")
		bool bRemoveDest;

	UPROPERTY(BlueprintReadWrite, AdvancedDisplay, Category = "HexGridPathFindingRequest")
		FGameplayTagContainer ExtraTags;


	bool IsValid();
};

/**
	Default pathfinder using manhattan distance to calculate cost.
	Inherit from USquarePathFinder or UHexagonPathFinder to customize pathfinding
*/
UCLASS(Blueprintable)
class UGridPathFinderAgent : public UObject
{
	GENERATED_BODY()

public:
	UGridPathFinderAgent();
	virtual ~UGridPathFinderAgent();

	UFUNCTION(BlueprintPure, Category = "HexGridPathFinder")
		UGridTile* GetStart() const;

	UFUNCTION(BlueprintPure, Category = "HexGridPathFinder")
		UGridTile* GetDestination() const;

	UFUNCTION(BlueprintPure, Category = "HexGridPathFinder")
		AActor* GetSender() const;

	UFUNCTION(BlueprintPure, Category = "HexGridPathFinder")
		AGridActor* GetGrid() const;

	UFUNCTION(BlueprintPure, Category = "HexGridPathFinder")
		const FGameplayTagContainer& GetExtraTags() const;

	/** If return false, the pawn can't move from 'Start' to 'Dest', the path has been blocked, and the cost will be ignore */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		bool IsReachable(const FGridPathFinderRequest InRequest);
	virtual bool IsReachable_Implementation(const FGridPathFinderRequest InRequest);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		void JumpingRequired(UGridTile* From, UGridTile* To, bool& JumpingRequired, float& UpperBound, bool bDrawDebug = false);
	virtual void JumpingRequired_Implementation(UGridTile* From, UGridTile* To, bool& JumpingRequired, float& UpperBound, bool bDrawDebug = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		int32 GetCost(UGridTile* From, UGridTile* To);
	virtual int32 GetCost_Implementation(UGridTile* From, UGridTile* To);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		int32 Heuristic(UGridTile* From, UGridTile* To);
	virtual int32 Heuristic_Implementation(UGridTile* From, UGridTile* To);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		bool CanPassThroughTile(UGridTile* FromTile, UGridTile* ToTile);
	virtual bool CanPassThroughTile_Implementation(UGridTile* FromTile, UGridTile* ToTile);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		bool CanStandOnTile(UGridTile* Tile);
	virtual bool CanStandOnTile_Implementation(UGridTile* Tile);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		void Reset();
	virtual void Reset_Implementation() {};

	UFUNCTION(BlueprintCallable, Category = "HexGridPathFinder")
		virtual bool FindPath(const FGridPathFinderRequest InRequest, TArray<UGridTile*>& Result);
	UFUNCTION(BlueprintCallable, Category = "HexGridPathFinder")
		virtual void GetReachableTiles(const FGridPathFinderRequest InRequest, TArray<UGridTile*>& Result);

	FGridPathFinderRequest Request;
};




UCLASS()
class UGridMovementAgent : public UGridPathFinderAgent
{
	GENERATED_BODY()


public:
	virtual bool CanPassThroughTile_Implementation(UGridTile* FromTile, UGridTile* ToTile) override;
	virtual bool CanStandOnTile_Implementation(UGridTile* Tile) override;

public:
	UPROPERTY()
		class UGridMovementComponent* GMC;
};


