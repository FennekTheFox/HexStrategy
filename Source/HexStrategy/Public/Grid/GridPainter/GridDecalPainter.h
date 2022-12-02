#pragma once

#include "GridPainter.h"
#include "GridDecalPainter.generated.h"

UCLASS(BlueprintType)
class UGridDecalPainter : public UGridPainter
{
	GENERATED_BODY()

public:
	UGridDecalPainter();

protected:
	virtual void UpdateTile(AGridTile* Tile) override;
	virtual void Clear() override;

	FRotator GetTileDecalRotation();

private:
	UPROPERTY()
		TMap<FIntVector, UDecalComponent*> CoordinatesToDecal;
	UPROPERTY()
		TMap<UDecalComponent*, UMaterialInstanceDynamic*> DecalToMID;
	UPROPERTY()
		UMaterialInterface* GridDecalMaterial;
};

