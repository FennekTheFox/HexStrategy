#pragma once

#include "GridPainter.h"
#include "GridDecalPainter.generated.h"

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class UGridDecalPainter : public UGridPainter
{
	GENERATED_BODY()

public:
	UGridDecalPainter();
	~UGridDecalPainter();


	void UpdateGrid() override;


	//void OnDestroy() override;

protected:
	virtual void UpdateTile(UGridTile* Tile) override;
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

