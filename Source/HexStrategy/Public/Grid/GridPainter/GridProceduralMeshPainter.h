// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridPainter/GridPainter.h"
#include "Grid/GridTile.h"

#include "GridProceduralMeshPainter.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic; 
class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class HEXSTRATEGY_API UGridProceduralMeshPainter : public UGridPainter
{
	GENERATED_BODY()
	
	UGridProceduralMeshPainter();

public:
	void UpdateGrid() override;


	void Clear() override;


	void UpdateTile(UGridTile* Tile) override;

public:
	UPROPERTY(VisibleAnywhere)
		class UProceduralMeshComponent* Comp;
	UPROPERTY(VisibleAnywhere)
		UMaterialInterface* MaterialBase;

	UPROPERTY(VisibleAnywhere)
		TMap<ETileDisplayState, UMaterialInstanceDynamic*> OutlineColours;
	UPROPERTY(VisibleAnywhere)
		TMap<ETileDisplayState, UMaterialInstanceDynamic*> FillColours;

	bool bIsInitialized = false;
};
