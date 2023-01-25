#pragma once

#include "HAL/Runnable.h"

//#include "GridBuilders.generated.h"


class AGridActor;
class UGridTile;

class  UGridTileBuilder : public FRunnable
{
public:
	UGridTileBuilder(AGridActor* _Grid, int32 _x)
	:Grid(_Grid)
	,private_x(_x)
	{};

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;


	TArray<UGridTile*> OutTiles;

private:
	void CreateTilesAtCoordinates(int32 x, int32 y);

	AGridActor* Grid = nullptr;
	UPROPERTY()
	int32 private_x = -1;
};


class  FGridNavigationBuilder : public FRunnable
{
public:
	FGridNavigationBuilder(AGridActor* _Grid, int32 _start, int32 _stop)
		:Grid(_Grid)
		, start(_start)
		, stop(_stop)
	{};

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;


	AGridActor* Grid = nullptr;
	int32 start = -1;
	int32 stop = -1;
};