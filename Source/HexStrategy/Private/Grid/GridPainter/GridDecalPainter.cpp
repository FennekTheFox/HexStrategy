#include "Grid/GridPainter/GridDecalPainter.h"
#include "Grid/GridTile.h"
#include "Grid/GridActor.h"
#include "Components/DecalComponent.h"

///Script/Engine.Material'/Game/Materials/M_GridDecalPainter.M_GridDecalPainter'

UGridDecalPainter::UGridDecalPainter()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("Script/Engine.Material'/Game/Materials/M_GridDecalPainter.M_GridDecalPainter'"));
	if (ensure(MaterialFinder.Object != NULL))
	{
		GridDecalMaterial = (UMaterialInterface*)MaterialFinder.Object;
	}
}

void UGridDecalPainter::UpdateTile(AGridTile* Tile)
{
	UWorld* World = GetWorld();
	ensureAlways(World);

	UDecalComponent* DecalComp;

	if (!CoordinatesToDecal.Contains(Tile->Coordinates))
	{
		//Tile decal doesn't exist yet, create it first
		DecalComp = NewObject<UDecalComponent>(this);
		DecalComp->RegisterComponentWithWorld(Grid->GetWorld());
		DecalComp->SetRelativeRotation(GetTileDecalRotation());
		CoordinatesToDecal.Add(Tile->Coordinates, DecalComp);

		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(GridDecalMaterial, this);
		DecalComp->SetDecalMaterial(MID);
	}
	else
	{
		//Tile already exists, update the tile decal here
		DecalComp = *CoordinatesToDecal.Find(Tile->Coordinates);
	}
	DecalComp->SetVisibility(Grid->bIsActive);
	DecalComp->DecalSize = FVector(1.0f, 1.0f, 1.0f) * Grid->TileSize * (1.f - Grid->TileSpacing) / 2;
	DecalComp->SetWorldLocation(Tile->GetActorLocation());

	UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(DecalComp->GetDecalMaterial());

	if (MID)
	{
		FTilePainterSettings* ColorSettings = Grid->GridPainterConfiguration.Find(Tile->GetDisplayState());

		MID->SetVectorParameterValue(TEXT("OutlineColor"), ColorSettings->OutlineColor);
		MID->SetVectorParameterValue(TEXT("FillColor"), ColorSettings->FillColor);
	}
}

FRotator UGridDecalPainter::GetTileDecalRotation()
{
	ensure(Grid);

	if (Grid)
	{
		switch (Grid->GridOrientation)
		{
		case EGridOrientation::Vertical: return FRotator(90.0f, 0.0f, 90.0f); break;
		case EGridOrientation::Horizontal: return FRotator(90.0f, 0.0f, 0.0f); break;
		default: return FRotator(0.0f, 0.0f, 0.0f);
		}
	}
	else return FRotator(0.0f, 0.0f, 0.0f);
}

void UGridDecalPainter::Clear()
{
	for (auto&& KVPair : CoordinatesToDecal)
	{
		UDecalComponent* Decal = KVPair.Value;
		Decal->DestroyComponent();
	}

	CoordinatesToDecal.Reset();
}
