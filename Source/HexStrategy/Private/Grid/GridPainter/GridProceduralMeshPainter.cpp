// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GridPainter/GridProceduralMeshPainter.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Grid/GridTile.h"
#include "Grid/GridActor.h"

UGridProceduralMeshPainter::UGridProceduralMeshPainter()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("/Script/Engine.Material'/Game/Materials/GridPainters/M_SingleColourChannel.M_SingleColourChannel'"));

	if (ensure(MatFinder.Succeeded()))
		MaterialBase = MatFinder.Object;
}

///Script/Engine.Material'/Game/NewMaterial.NewMaterial'

void UGridProceduralMeshPainter::UpdateGrid()
{
	//if (!bIsInitialized)
	//{
	for (ETileDisplayState State : TEnumRange<ETileDisplayState>())
	{
		UMaterialInstanceDynamic* OutlineMID = UMaterialInstanceDynamic::Create(MaterialBase, this);
		OutlineMID->SetVectorParameterValue(FName("Colour"), Grid->GridPainterConfiguration->Colours.Find(State)->OutlineColor);
		UMaterialInstanceDynamic* FillMID = UMaterialInstanceDynamic::Create(MaterialBase, this);
		FillMID->SetVectorParameterValue(FName("Colour"), Grid->GridPainterConfiguration->Colours.Find(State)->FillColor);
		OutlineColours.Add(State, OutlineMID);
		FillColours.Add(State, FillMID);
	}

	bIsInitialized = true;
	//}


	if (!Comp)
	{
		Comp = NewObject<UProceduralMeshComponent>(Grid);
		Comp->RegisterComponent();
	}
	Comp->SetCastShadow(false);
	Comp->SetReceivesDecals(false);

	Super::UpdateGrid();
}

void UGridProceduralMeshPainter::Clear()
{
	if (Comp)
		Comp->ClearAllMeshSections();
}

void UGridProceduralMeshPainter::UpdateTile(UGridTile* Tile)
{
	int32 SectionIndex = Grid->GridTiles.Find(Tile);
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;


	for (int i = 0; i < 6; i++)
	{

		Vertices.Add(Tile->WorldLocation + UKismetMathLibrary::RotateAngleAxis(
			FVector::UpVector + UKismetMathLibrary::RotateAngleAxis
			(FVector((Grid->TileSize * (1 - Grid->TileSpacing)) / 2.f, 0.f, 0.f),
				60 * i,
				FVector::UpVector),
			(Grid->GridOrientation == EGridOrientation::Vertical ? 30.f : 0.f),
			FVector::UpVector
		)
		);

		Vertices.Add(Tile->WorldLocation + UKismetMathLibrary::RotateAngleAxis(
			FVector::UpVector + UKismetMathLibrary::RotateAngleAxis
			(FVector((Grid->TileSize * (0.95f - Grid->TileSpacing)) / 2.f, 0.f, 0.f),
				60 * i,
				FVector::UpVector),
			(Grid->GridOrientation == EGridOrientation::Vertical ? 30.f : 0.f),
			FVector::UpVector
		)
		);
	}

	for (int i = 0; i < 6; i++)
	{
		Triangles.Add(2 * i);
		Triangles.Add((2 * i + 1) % Vertices.Num());
		Triangles.Add((2 * (i + 1)) % Vertices.Num());

		Triangles.Add((2 * (i + 1)) % Vertices.Num());
		Triangles.Add((2 * i + 1) % Vertices.Num());
		Triangles.Add((2 * (i + 1) + 1) % Vertices.Num());
	}

	Comp->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, false);

	Comp->SetMaterial(SectionIndex, *OutlineColours.Find(Tile->GetDisplayState()));

	int32 SectionIndex2 = Grid->GridTiles.Num() + Grid->GridTiles.Find(Tile);

	TArray<FVector> Vertices2;
	TArray<int32> Triangles2;

	for (int i = 0; i < 6; i++)
	{
		Vertices2.Add(Tile->WorldLocation + UKismetMathLibrary::RotateAngleAxis(
			FVector::UpVector + UKismetMathLibrary::RotateAngleAxis
			(FVector((Grid->TileSize * (0.95f - Grid->TileSpacing)) / 2.f, 0.f, 0.f),
				60 * i,
				FVector::UpVector),
			(Grid->GridOrientation == EGridOrientation::Vertical ? 30.f : 0.f),
			FVector::UpVector
		));
	}


	Triangles2.Add(0);
	Triangles2.Add(2);
	Triangles2.Add(1);

	Triangles2.Add(0);
	Triangles2.Add(3);
	Triangles2.Add(2);

	Triangles2.Add(3);
	Triangles2.Add(5);
	Triangles2.Add(4);

	Triangles2.Add(5);
	Triangles2.Add(3);
	Triangles2.Add(0);

	Comp->CreateMeshSection(SectionIndex2, Vertices2, Triangles2, Normals, UV0, VertexColors, Tangents, false);

	Comp->SetMaterial(SectionIndex2, *FillColours.Find(Tile->GetDisplayState()));
}
