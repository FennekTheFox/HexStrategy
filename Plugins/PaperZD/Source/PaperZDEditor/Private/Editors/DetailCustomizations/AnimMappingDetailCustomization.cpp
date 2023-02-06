// Copyright 2023, Fennel Fox, Discord @Fennel#7727. Feel free to use this for anything honestly, go nuts.

#include "AnimMappingDetailCustomization.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Editors/Slate/SPaperZDAnimDataSourceGrid.h"
#include "IPropertyUtilities.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Input/SNumericEntryBox.h" 	
#include "ScopedTransaction.h"
#include "PaperZDAnimMapping.h"
#include <SPinTypeSelector.h>

#define MIN_NUM_NODES 3
#define MAX_NUM_NODES 16

#define LOCTEXT_NAMESPACE "ZDAnimSequenceDetailCustomization"

TSharedRef<IDetailCustomization> FPaperZDAnimMappingDetailCustomization::MakeInstance()
{
	return MakeShareable(new FPaperZDAnimMappingDetailCustomization);
}

void FPaperZDAnimMappingDetailCustomization::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder)
{
	DetailBuilderPtr = DetailBuilder;
	CustomizeDetails(*DetailBuilder);
}

void FPaperZDAnimMappingDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> SelectedObjectsList;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjectsList);

	// Multi selection is disabled for this
	UPaperZDAnimMapping* AnimMapping = Cast<UPaperZDAnimMapping>(SelectedObjectsList[0].Get());
	AnimMappingPtr = AnimMapping;
	if (AnimMapping == nullptr || SelectedObjectsList.Num() > 1)
	{
		return;
	}


	//Make sure the AnimSequence has a valid number of nodes
	if (!InitDataSourceHandleForAnimMapping(AnimMapping))
	{
		return;
	}
	

	//Create a row for the grid to live in
	FDetailWidgetRow& EnumPickerRow = DetailBuilder.EditCategory("AnimMapping").AddCustomRow(FText::FromString("DataSource"));

	EnumPickerRow
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Enum Base"))
				.Margin(FMargin(0.0f, 0.0f, 0.0f, 2.0f))
			]
			+SHorizontalBox::Slot()
			[
				SNew(SPinTypeSelector, FGetPinTypeTree::CreateUObject(GetDefault<UEdGraphSchema_K2>(), &UEdGraphSchema_K2::GetVariableTypeTree))
				.Schema(GetDefault<UEdGraphSchema_K2>())
				.OnPinTypePreChanged(this, &FPaperZDAnimMappingDetailCustomization::OnPrePinInfoChange)
				.OnPinTypeChanged(this, &FPaperZDAnimMappingDetailCustomization::PinInfoChanged)
				//.TargetPinType(this, &FPaperZDAnimMappingDetailCustomization::GetPinType)
			]
		];
}

bool FPaperZDAnimMappingDetailCustomization::InitDataSourceHandleForAnimMapping(UPaperZDAnimMapping* AnimMapping)
{
	return true;
}




EVisibility FPaperZDAnimMappingDetailCustomization::GetDetailVisibility(int32 Index) const
{
	return Index == SelectedNodeIndex ? EVisibility::Visible : EVisibility::Hidden;
}

int32 FPaperZDAnimMappingDetailCustomization::GetNumNodes() const
{
	uint32 NumNodes = 0;
	ArrayHandle->GetNumElements(NumNodes);
	return static_cast<int32>(NumNodes);
}

uint32 FPaperZDAnimMappingDetailCustomization::GetNumNodesUnsigned() const
{
	uint32 NumNodes = 0;
	ArrayHandle->GetNumElements(NumNodes);
	return NumNodes;
}

TOptional<uint32> FPaperZDAnimMappingDetailCustomization::GetValueForNumNodesNumericEntry() const
{
	return TOptional<uint32>(GetNumNodesUnsigned());
}


void FPaperZDAnimMappingDetailCustomization::OnCommitedNumNodes(uint32 NewValue, ETextCommit::Type CommitType)
{
	if (NewValue != GetNumNodesUnsigned() && NewValue >= MIN_NUM_NODES && NewValue <= MAX_NUM_NODES)
	{
		FScopedTransaction Transaction(NSLOCTEXT("PaperZD", "AnimSequenceSetNumNodes", "Set Number of Nodes"));
		AnimMappingPtr->Modify();
		//SetDataSourceNum(NewValue);
		ForceRefresh();
	}
}

void FPaperZDAnimMappingDetailCustomization::ForceRefresh()
{
	//Need the detail layout builder to not hold a reference, otherwise the details view crashes due to the DetailLayout pointer not being unique
	if (DetailBuilderPtr.IsValid())
	{
		IDetailLayoutBuilder* LayoutBuilder = DetailBuilderPtr.Pin().Get();
		LayoutBuilder->ForceRefreshDetails();
	}
}

void FPaperZDAnimMappingDetailCustomization::PostUndo(bool bSuccess)
{
	//Possibility that the post operation data source requires more nodes than we have available for use
	if (!CheckPostUndoRedoSupport())
	{
		ForceRefresh();
	}
}

void FPaperZDAnimMappingDetailCustomization::PostRedo(bool bSuccess)
{
	//Possibility that the post operation data source requires more nodes than we have available for use
	if (!CheckPostUndoRedoSupport())
	{
		ForceRefresh();
	}
}

bool FPaperZDAnimMappingDetailCustomization::CheckPostUndoRedoSupport() const
{
	//After an UNDO/REDO the property handle doesn't return a valid entry yet, hence we need to query the property directly
	bool bSupported = false;

	return bSupported;
}


FEdGraphPinType FPaperZDAnimMappingDetailCustomization::GetPinType()
{
	return FEdGraphPinType();
}


void FPaperZDAnimMappingDetailCustomization::PinInfoChanged(const FEdGraphPinType& PinType)
{
	if (UEnum* EnumType = Cast<UEnum>(PinType.PinSubCategoryObject.Get()))
	{
		UE_LOG(LogTemp, Log, TEXT("Foo"))
		AnimMappingPtr->SetEnum(EnumType);
	}
}


void FPaperZDAnimMappingDetailCustomization::OnPrePinInfoChange(const FEdGraphPinType& PinType)
{

}

#undef LOCTEXT_NAMESPACE
