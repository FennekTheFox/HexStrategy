// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridPainter/GridPainter.h"
#include "GridInstancedMeshPainter.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class HEXSTRATEGY_API UGridInstancedMeshPainter : public UGridPainter
{
	GENERATED_BODY()


	UGridInstancedMeshPainter();
	~UGridInstancedMeshPainter();

public:
	void UpdateGrid() override;


	void Clear() override;


	void UpdateTile(UGridTile* Tile) override;
	

public:
	UPROPERTY()
		TMap<ETileDisplayState, UInstancedStaticMeshComponent*> Comps;

	UPROPERTY(VisibleAnywhere)
		UMaterialInterface* MaterialBase;
	UPROPERTY(VisibleAnywhere)
		class UStaticMesh* Mesh;

	UPROPERTY()
		TMap<ETileDisplayState, UMaterialInstanceDynamic*> OutlineColours;
	UPROPERTY()
		TMap<ETileDisplayState, UMaterialInstanceDynamic*> FillColours;
	UPROPERTY()
		TMap<UGridTile*, ETileDisplayState> DisplayedStates;
	UPROPERTY()
		TMap<UGridTile*, int32> TileIndices;
};
