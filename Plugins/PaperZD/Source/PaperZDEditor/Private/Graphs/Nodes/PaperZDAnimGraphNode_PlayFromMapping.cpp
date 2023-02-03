// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_PlayFromMapping.h"
#include "Kismet2/CompilerResultsLog.h"

#define LOCTEXT_NAMESPACE "ZDNodes"

UPaperZDAnimGraphNode_PlayFromMapping::UPaperZDAnimGraphNode_PlayFromMapping()
	: Super()
{}

FString UPaperZDAnimGraphNode_PlayFromMapping::GetNodeCategory() const
{
	return TEXT("Animation");
}

void UPaperZDAnimGraphNode_PlayFromMapping::ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog)
{
	//We should have both animation links connected
	UEdGraphPin* MappingPin = FindPin(GET_MEMBER_NAME_CHECKED(FPaperZDAnimNode_PlayFromMapping, AnimMapping), EGPD_Input);
	check(MappingPin);

	if (MappingPin->LinkedTo.Num() == 0)
	{
		MessageLog.Error(TEXT("@@ has nothing bound to its mapping pin"), this);
	}

	//We should have both animation links connected
	UEdGraphPin* KeyPin = FindPin(GET_MEMBER_NAME_CHECKED(FPaperZDAnimNode_PlayFromMapping, Key), EGPD_Input);
	check(KeyPin);

	if (KeyPin->LinkedTo.Num() == 0)
	{
		MessageLog.Error(TEXT("@@ has nothing bound to its key pin"), this);
	}
}

//FLinearColor UPaperZDAnimGraphNode_PlayFromMapping::GetNodeTitleColor() const
//{
//	return FLinearColor::White;
//}
//
//FText UPaperZDAnimGraphNode_PlayFromMapping::GetNodeTitle(ENodeTitleType::Type TitleType) const
//{
//	return LOCTEXT("SetDirectionalityNodeTitle", "Set Directionality2");
//}




//// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.
//
//#include "PaperZDAnimGraphNode_PlayFromMapping.h"
//#include "Graphs/PaperZDAnimGraph.h"
//#include "AnimSequences/PaperZDAnimSequence.h"
//#include "AnimSequences/Sources/PaperZDAnimationSource.h"
//#include "BlueprintNodeSpawner.h"
//#include "BlueprintActionDatabaseRegistrar.h"
//#include "Modules/ModuleManager.h"
//#include "AssetRegistry/ARFilter.h"
//#include "AssetRegistry/AssetRegistryModule.h"
//#include "Kismet2/CompilerResultsLog.h"
//#include "PaperZDAnimBP.h"
//#include "AnimNodes/PaperZDAnimNode_PlayFromMapping.h"
//#include "PaperZDAnimMapping.h"
//
// 
//void UPaperZDAnimGraphNode_PlayFromMapping::PreloadRequiredAssets()
//{
//	PreloadObject(MappingNode.GetAnimSequence());
//
//	Super::PreloadRequiredAssets();
//}



FLinearColor UPaperZDAnimGraphNode_PlayFromMapping::GetNodeTitleColor() const
{
	return FColor(100, 100, 200);
}


FText UPaperZDAnimGraphNode_PlayFromMapping::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("PlayFrpmMapping_Title_NoSequence", "Play From Mapping");

}

//
//FText UPaperZDAnimGraphNode_PlayFromMapping::GetNodeTitleBySequenceName(FName SequenceName) const
//{
//	//Construct the path
//	FFormatNamedArguments Args;
//	FText Name = FText::FromName(SequenceName);
//	Args.Add(TEXT("Name"), Name);
//	return FText::Format(LOCTEXT("PlayFromMapping_Title", "Play {Name}"), Args);
//}
//
//FText UPaperZDAnimGraphNode_PlayFromMapping::GetNodeTooltipBySequencePath(FString SequencePath) const
//{
//	//Construct the path
//	FFormatNamedArguments Args;
//	FText Path = FText::FromString(SequencePath);
//	Args.Add(TEXT("Path"), Path);
//	return FText::Format(LOCTEXT("PlaySequence_Tooltip", "Play {Path}"), Args);
//}
//
//void UPaperZDAnimGraphNode_PlayFromMapping::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
//{
//	auto LoadedAssetSetup = [](UEdGraphNode* NewNode, bool /*bIsTemplateNode*/, TWeakObjectPtr<UPaperZDAnimMapping> MappingPtr)
//	{
//		UPaperZDAnimGraphNode_PlayFromMapping* GraphNode = CastChecked<UPaperZDAnimGraphNode_PlayFromMapping>(NewNode);
//		GraphNode->MappingNode.AnimMapping = MappingPtr.Get();
//	};
//
//	auto UnloadedAssetSetup = [](UEdGraphNode* NewNode, bool bIsTemplateNode, const FAssetData AssetData)
//	{
//		UPaperZDAnimGraphNode_PlayFromMapping* GraphNode = CastChecked<UPaperZDAnimGraphNode_PlayFromMapping>(NewNode);
//		if (!bIsTemplateNode)
//		{
//			UPaperZDAnimMapping* Mapping = Cast<UPaperZDAnimMapping>(AssetData.GetAsset());
//			check(Mapping != nullptr);
//			GraphNode->MappingNode.AnimMapping = Mapping;
//		}
//		else
//		{
//			//The asset is not loaded, but we can get the Tag value that represents the AnimBP
//			AssetData.GetTagValue(UPaperZDAnimSequence::GetAnimSourceMemberName(), GraphNode->PROTO_AnimSourceTag);
//		}
//	};
//		
//	const UObject* QueryObject = ActionRegistrar.GetActionKeyFilter();
//	if (QueryObject == nullptr)
//	{
//		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
//		// define a filter to help in pulling the AnimSequence asset data from the registry
//		FARFilter Filter;
//		Filter.ClassPaths.Add(UPaperZDAnimSequence::StaticClass()->GetClassPathName());
//		Filter.bRecursiveClasses = true;
//		// Find matching assets and add an entry for each one
//		TArray<FAssetData> SequenceList;
//		AssetRegistryModule.Get().GetAssets(Filter, /*out*/SequenceList);
//
//		//Register ~every~ AnimSequence as an action that can possibly be added to the graph (will be filtered out in later stages)
//		for (const FAssetData& Asset : SequenceList)
//		{
//			UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
//			if (Asset.IsAssetLoaded())
//			{
//				UPaperZDAnimMapping* Mapping = Cast<UPaperZDAnimMapping>(Asset.GetAsset());
//				if (Mapping)
//				{
//					TWeakObjectPtr<UPaperZDAnimMapping> MappingPtr = MakeWeakObjectPtr(const_cast<UPaperZDAnimMapping*>(Mapping));
//					NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(LoadedAssetSetup, MappingPtr);
//					NodeSpawner->DefaultMenuSignature.MenuName = GetNodeTitleBySequenceName(Mapping->GetFName());
//					NodeSpawner->DefaultMenuSignature.Tooltip = GetNodeTooltipBySequencePath(Mapping->GetPathName());
//
//					ActionRegistrar.AddBlueprintAction(Asset, NodeSpawner);
//				}
//			}
//			else
//			{
//				NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(UnloadedAssetSetup, Asset);
//				NodeSpawner->DefaultMenuSignature.MenuName = GetNodeTitleBySequenceName(Asset.AssetName);
//				NodeSpawner->DefaultMenuSignature.Tooltip = GetNodeTooltipBySequencePath(Asset.GetObjectPathString());
//
//				ActionRegistrar.AddBlueprintAction(Asset, NodeSpawner);
//			}
//		}
//	}
//	else if (const UPaperZDAnimMapping* Mapping = Cast<const UPaperZDAnimMapping>(QueryObject))
//	{
//		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
//
//		TWeakObjectPtr<UPaperZDAnimMapping> MappingPtr = MakeWeakObjectPtr(const_cast<UPaperZDAnimMapping*>(Mapping));
//		NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(LoadedAssetSetup, MappingPtr);
//		NodeSpawner->DefaultMenuSignature.MenuName = GetNodeTitleBySequenceName(MappingPtr->GetFName());
//		NodeSpawner->DefaultMenuSignature.Tooltip = GetNodeTooltipBySequencePath(MappingPtr->GetPathName());
//
//		ActionRegistrar.AddBlueprintAction(QueryObject, NodeSpawner);
//	}
//	else if (QueryObject == GetClass())
//	{
//		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
//		// define a filter to help in pulling the AnimSequence asset data from the registry
//		FARFilter Filter;
//		Filter.ClassPaths.Add(UPaperZDAnimSequence::StaticClass()->GetClassPathName());
//		Filter.bRecursiveClasses = true;
//		// Find matching assets and add an entry for each one
//		TArray<FAssetData> SequenceList;
//		AssetRegistryModule.Get().GetAssets(Filter, /*out*/SequenceList);
//
//		//Register ~every~ AnimSequence as an action that can possibly be added to the graph (will be filtered out in later stages)
//		for (const FAssetData& Asset : SequenceList)
//		{
//			if (!Asset.IsAssetLoaded())
//			{
//				UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
//				NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(UnloadedAssetSetup, Asset);
//				NodeSpawner->DefaultMenuSignature.MenuName = GetNodeTitleBySequenceName(Asset.AssetName);
//				NodeSpawner->DefaultMenuSignature.Tooltip = GetNodeTooltipBySequencePath(Asset.GetObjectPathString());
//
//				ActionRegistrar.AddBlueprintAction(Asset, NodeSpawner);
//			}
//		}
//	}
//}
//
//bool UPaperZDAnimGraphNode_PlayFromMapping::IsActionFilteredOut(class FBlueprintActionFilter const& Filter)
//{
//	//We can only be used contextually by animations that share the same animation source
//	if (MappingNode.AnimMapping)
//	{
//		for (UBlueprint* Blueprint : Filter.Context.Blueprints)
//		{
//			UPaperZDAnimBP* AnimBP = Cast<UPaperZDAnimBP>(Blueprint);
//			if (!AnimBP || AnimBP->GetSupportedAnimationSource() != MappingNode.AnimMapping->GetAnimSource())
//			{
//				return true;
//			}
//		}
//	}
//	else if (!PROTO_AnimSourceTag.IsEmpty())
//	{
//		//Asset is unloaded, we will instead check against the value that was filled on the PROTO node
//		for (UBlueprint* Blueprint : Filter.Context.Blueprints)
//		{
//			UPaperZDAnimBP* AnimBP = Cast<UPaperZDAnimBP>(Blueprint);
//			UPaperZDAnimationSource* AnimSource = AnimBP ? AnimBP->GetSupportedAnimationSource() : nullptr;
//			if (!AnimSource || FAssetData(AnimSource).GetExportTextName() != PROTO_AnimSourceTag)
//			{
//				return true;
//			}
//		}
//	}
//	else
//	{
//		//NULL-node, had no AnimSequence info and wasn't a PROTO node
//		return true;
//	}
//
//	//Node passed all verifications, can be used
//	return false;
//}
//
//void UPaperZDAnimGraphNode_PlayFromMapping::PostPlacedNewNode()
//{
//	Super::PostPlacedNewNode();
//
//	//Transitional anim graph requires this node to be forced as non-looping
//	if (GetAnimGraph()->IsTransitionalGraph())
//	{
//		MappingNode.bLoopAnimation = false;
//	}
//}
//
//void UPaperZDAnimGraphNode_PlayFromMapping::PostPasteNode()
//{
//	Super::PostPasteNode();
//
//	//Transitional anim graph requires this node to be forced as non-looping
//	if (GetAnimGraph()->IsTransitionalGraph())
//	{
//		//Pin should be hidden first, as the default value will be copied into the property when hidden
//		SetShowPinForProperty(GET_MEMBER_NAME_CHECKED(FPaperZDAnimNode_PlayFromMapping, bLoopAnimation), false);
//		MappingNode.bLoopAnimation = false;
//	}
//}
//
//void UPaperZDAnimGraphNode_PlayFromMapping::ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog)
//{
//	Super::ValidateAnimNodeDuringCompilation(ForAnimBP, MessageLog);
//
//	//We should always have an AnimSequence linked to us
//	UPaperZDAnimSequence* CheckedSequence = MappingNode.GetAnimSequence();
//	UEdGraphPin* MappingPin = FindPin(GET_MEMBER_NAME_STRING_CHECKED(FPaperZDAnimNode_PlayFromMapping, AnimMapping));
//
//	if (MappingPin != nullptr && CheckedSequence == nullptr)
//	{
//		//No Sequence on the AnimNode, the pin could have it added
//		CheckedSequence = Cast<UPaperZDAnimSequence>(MappingPin->DefaultObject);
//	}
//
//	//If the sequence is null, then we might have the node bound to it
//	if (CheckedSequence == nullptr)
//	{
//		// we may have a connected node or binding
//		if (MappingPin == nullptr || (MappingPin->LinkedTo.Num() == 0))
//		{
//			MessageLog.Error(TEXT("@@ references an unknown sequence"), this);
//		}
//	}
//	else
//	{
//		//If we do have a sequence, we should make sure that it is compatible to the AnimBP that we are on
//		if (CheckedSequence->GetAnimSource() != ForAnimBP->GetSupportedAnimationSource())
//		{
//			MessageLog.Error(TEXT("@@ references sequence that is not compatible with the current AnimBP"), this);
//		}
//
//		//If this node is inside a transitional anim graph, we cannot loop the animation
//		UPaperZDAnimGraph* AnimGraph = Cast<UPaperZDAnimGraph>(GetGraph());
//		if (AnimGraph && AnimGraph->IsTransitionalGraph() && MappingNode.bLoopAnimation)
//		{
//			MessageLog.Warning(TEXT("@@ is set to looping on a transitional anim graph, this is not supported. Will compile as non-looping animation."), this);
//			MappingNode.bLoopAnimation = false;
//		}
//	}
//
//}
//
#undef LOCTEXT_NAMESPACE