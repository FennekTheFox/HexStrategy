#pragma once

#include "GameplayTagContainer.h"
#include "GridPathFinder.generated.h"

class UGridPathFinderAgent;


struct FWidthSearchHelper
{
public:
	FWidthSearchHelper(AGridTile* Origin, AGridActor* InGrid, UGridPathFinderAgent* InAgent)
		:Start(Origin)
		, Grid(InGrid)
		, Agent(InAgent)
	{
		Comparer.TravelCost = &TravelCost;
		TravelCost.FindOrAdd(Start, 0.f);
		OpenSet.HeapPush(Start, Comparer);
	};

	bool Step();
	void GetAllReachableTiles(TArray<AGridTile*>& OutTiles);
public:
	bool Success = false;
private:
	struct FComparer
	{
		bool operator()(const AGridTile& L, const AGridTile& R) const
		{
			int32 LTravelCost = TravelCost->Contains(&L) ? TravelCost->FindChecked(&L) : TNumericLimits<int32>::Max();
			int32 RTravelCost = TravelCost->Contains(&R) ? TravelCost->FindChecked(&R) : TNumericLimits<int32>::Max();
			return LTravelCost <= RTravelCost;
		}
		TMap<AGridTile*, int32>* TravelCost;
	} Comparer;

	TArray<AGridTile*> OpenSet;
	TSet<AGridTile*> ReachedTiles;
	TMap<AGridTile*, int32> TravelCost;
	TMap<AGridTile*, AGridTile*> CameFrom;
	AGridTile* Start;
	AGridActor* Grid;
	UGridPathFinderAgent* Agent;
};

struct FAStarHelper
{
public:
	FAStarHelper(AGridTile* From, AGridTile* To, AGridActor* InGrid, UGridPathFinderAgent* InAgent)
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
	void CollectPath(TArray<AGridTile*>& Result) const;
public:
	bool Success = false;
private:
	int32 DistanceHeuristic(AGridTile* Probe, AGridTile* Target);

	struct FComparer
	{
		bool operator()(const AGridTile& L, const AGridTile& R) const;
		TMap<AGridTile*, int32>* FCost;
	} Comparer;

	TArray<AGridTile*> OpenSet;
	TSet<AGridTile*> CloseSet;
	TMap<AGridTile*, int32> FCost;
	TMap<AGridTile*, int32> TravelCost;
	TMap<AGridTile*, AGridTile*> CameFrom;
	AGridTile* Start;
	AGridTile* Goal;
	AGridActor* Grid;
	UGridPathFinderAgent* Agent;
};


class AGridActor;
class AGridTile;

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
		AGridTile* Start;

	UPROPERTY(BlueprintReadWrite, Category = "HexGridPathFindingRequest")
		AGridTile* Goal;

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
		AGridTile* GetStart() const;

	UFUNCTION(BlueprintPure, Category = "HexGridPathFinder")
		AGridTile* GetDestination() const;

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
		void JumpingRequired(AGridTile* From, AGridTile* To, bool& JumpingRequired, float& UpperBound, bool bDrawDebug = false);
	virtual void JumpingRequired_Implementation(AGridTile* From, AGridTile* To, bool& JumpingRequired, float& UpperBound, bool bDrawDebug = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		int32 GetCost(AGridTile* From, AGridTile* To);
	virtual int32 GetCost_Implementation(AGridTile* From, AGridTile* To);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		int32 Heuristic(AGridTile* From, AGridTile* To);
	virtual int32 Heuristic_Implementation(AGridTile* From, AGridTile* To);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		bool CanPassThroughTile(AGridTile* FromTile, AGridTile* ToTile);
	virtual bool CanPassThroughTile_Implementation(AGridTile* FromTile, AGridTile* ToTile);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		bool CanStandOnTile(AGridTile* Tile);
	virtual bool CanStandOnTile_Implementation(AGridTile* Tile);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HexGridPathFinder")
		void Reset();
	virtual void Reset_Implementation() {};

	UFUNCTION(BlueprintCallable, Category = "HexGridPathFinder")
		virtual bool FindPath(const FGridPathFinderRequest InRequest, TArray<AGridTile*>& Result);
	UFUNCTION(BlueprintCallable, Category = "HexGridPathFinder")
		virtual void GetReachableTiles(const FGridPathFinderRequest InRequest, TArray<AGridTile*>& Result);

	FGridPathFinderRequest Request;
};




UCLASS()
class UGridMovementAgent : public UGridPathFinderAgent
{
	GENERATED_BODY()


public:
	virtual bool CanPassThroughTile_Implementation(AGridTile* FromTile, AGridTile* ToTile) override;
	virtual bool CanStandOnTile_Implementation(AGridTile* Tile) override;

public:
	UPROPERTY()
		class UGridMovementComponent* GMC;
};


