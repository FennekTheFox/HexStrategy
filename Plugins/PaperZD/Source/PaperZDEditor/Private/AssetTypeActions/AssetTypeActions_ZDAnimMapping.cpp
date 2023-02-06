// Copyright 2023, Fennel Fox, Discord @Fennel#7727. Feel free to use this for anything honestly, go nuts.

#include "AssetTypeActions_ZDAnimMapping.h"

#include "Editors/Util/PaperZDEditorStyle.h"
#include "Toolkits/IToolkitHost.h"
#include "Modules/ModuleManager.h"
#include "PaperZDEditor.h"
#include "PaperZDAnimMapping.h"

#define LOCTEXT_NAMESPACE "AssetTypeActionsPaperZDAnimationMapping"

//static defines
const FName FAssetTypeActions_ZDAnimMapping::CategoryKey = FName("PaperZD_AssetActionsKey");
const FText FAssetTypeActions_ZDAnimMapping::CategoryDisplayName = FText::FromString("PaperZD");

FAssetTypeActions_ZDAnimMapping::FAssetTypeActions_ZDAnimMapping(EAssetTypeCategories::Type InAssetCategory) :
MyAssetCategory(InAssetCategory)
{}

UClass* FAssetTypeActions_ZDAnimMapping::GetSupportedClass() const
{
	return UPaperZDAnimMapping::StaticClass();
}

//void FAssetTypeActions_ZDAnimMapping::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
//{
//	//EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
//	//FPaperZDEditorModule* EditorModule = &FModuleManager::LoadModuleChecked<FPaperZDEditorModule>("PaperZDEditor");
//
//	////Open the asset editor for the selected objects
//	//for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
//	//{
//	//	UPaperZDAnimMapping* AnimSource = Cast<UPaperZDAnimMapping>(*ObjIt);
//	//	if (AnimSource)
//	//	{
//	//		EditorModule->CreateAnimationSourceEditor(Mode, EditWithinLevelEditor, AnimSource);
//	//	}
//	//}
//}

uint32 FAssetTypeActions_ZDAnimMapping::GetCategories()
{
	return MyAssetCategory;
}


//void FAssetTypeActions_ZDAnimMapping::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
//{
//
//}


//bool FAssetTypeActions_ZDAnimMapping::HasActions(const TArray<UObject*>& InObjects) const
//{
//
//}

#undef LOCTEXT_NAMESPACE
