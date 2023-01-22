// Fill out your copyright notice in the Description page of Project Settings.


#include "GridInstancedMeshPainter.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Grid/GridActor.h"
#include "Engine/StaticMesh.h"

UGridInstancedMeshPainter::UGridInstancedMeshPainter()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("/Script/Engine.Material'/Game/Materials/GridPainters/M_SingleColourChannel.M_SingleColourChannel'"));

	if (ensure(MatFinder.Succeeded()))
		MaterialBase = MatFinder.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/Materials/GridPainters/Tile.Tile'"));

	if (ensure(MeshFinder.Succeeded()))
		Mesh = MeshFinder.Object;
}

UGridInstancedMeshPainter::~UGridInstancedMeshPainter()
{
	for (auto&& KVPair : Comps)
	{
		KVPair.Value->UnregisterComponent();
	}
}

void UGridInstancedMeshPainter::UpdateGrid()
{
	for (ETileDisplayState State : TEnumRange<ETileDisplayState>())
	{
		UMaterialInstanceDynamic* OutlineMID = UMaterialInstanceDynamic::Create(MaterialBase, this);
		OutlineMID->SetVectorParameterValue(FName("Colour"), Grid->GridPainterConfiguration->Colours.Find(State)->OutlineColor);
		UMaterialInstanceDynamic* FillMID = UMaterialInstanceDynamic::Create(MaterialBase, this);
		FillMID->SetVectorParameterValue(FName("Colour"), Grid->GridPainterConfiguration->Colours.Find(State)->FillColor);
		OutlineColours.Add(State, OutlineMID);
		FillColours.Add(State, FillMID);


		if (!Comps.Find(State))
		{
			UInstancedStaticMeshComponent* Comp = NewObject<UInstancedStaticMeshComponent>(Grid);
			Comp->RegisterComponent();
			Comps.Add(State, Comp);
		}


		UInstancedStaticMeshComponent* Comp = *Comps.Find(State);
		Comp->SetMaterial(0, FillMID);
		Comp->SetMaterial(1, OutlineMID);
		Comp->SetStaticMesh(Mesh);
		Comp->SetCastShadow(false);
		Comp->SetReceivesDecals(false);
		Comp->SetVisibility(Grid->bIsActive);
	}

	Super::UpdateGrid();
}

void UGridInstancedMeshPainter::Clear()
{
	for (auto&& KVPair : Comps)
	{
		UInstancedStaticMeshComponent* Comp = KVPair.Value;

		if (ensure(Comp))
		{
			Comp->ClearInstances();
		}
	}
}


void UGridInstancedMeshPainter::UpdateTile(UGridTile* Tile)
{
	ETileDisplayState* OldState = DisplayedStates.Find(Tile);
	ETileDisplayState NewState = Tile->GetDisplayState();

	if (OldState && (*OldState != NewState))
	{
		//Remove the old instance since it exists and the state is different
		int32 OldIndex = *TileIndices.Find(Tile);

		UInstancedStaticMeshComponent* OldComp = *Comps.Find(*OldState);
		OldComp->RemoveInstance(OldIndex);
	}

	UInstancedStaticMeshComponent* RelevantComp = *Comps.Find(NewState);

	ensure(RelevantComp);

	FTransform InstanceTransform;
	InstanceTransform.SetTranslation(Tile->WorldLocation + FVector::UpVector);
	InstanceTransform.SetScale3D(FVector(1-Grid->TileSpacing));

	int32 newIndex = RelevantComp->AddInstance(InstanceTransform);

	DisplayedStates.Add(Tile, NewState);
	TileIndices.Add(Tile, newIndex);
}