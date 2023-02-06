// Copyright 2023, Fennel Fox, Discord @Fennel#7727. Feel free to use this for anything honestly, go nuts.

#include "PaperZDAnimMappingFactory.h"
#include "PaperZDAnimMapping.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "EditorStyleSet.h"
#include "Modules/ModuleManager.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "Misc/FeedbackContext.h"
#include "Editor.h"
#include "AnimSequences/PaperZDAnimSequence.h"


#define LOCTEXT_NAMESPACE "PaperZDAnimationMappingFactory"

UPaperZDAnimMappingPFactory::UPaperZDAnimMappingPFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = false;
	SupportedClass = UPaperZDAnimMapping::StaticClass();
	TargetAnimSource = nullptr;
}

bool UPaperZDAnimMappingPFactory::ConfigureProperties()
{
	// Optionally select the AnimSource if it hasn't been provided yet
	if (TargetAnimSource == nullptr)
	{
		// Load the content browser module to display an asset picker
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		FAssetPickerConfig AssetPickerConfig;

		/** The asset picker will only show AnimBP */
		AssetPickerConfig.Filter.ClassNames.Add(UPaperZDAnimationSource::StaticClass()->GetFName());
		AssetPickerConfig.Filter.bRecursiveClasses = true;

		/** The delegate that fires when an asset was selected */
		AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateUObject(this, &UPaperZDAnimMappingPFactory::OnTargetAnimSourceSelected);

		/** The default view mode should be a list view */
		AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;

		PickerWindow = SNew(SWindow)
			.Title(LOCTEXT("PaperZDAnimationMappingFactory", "Pick Parent Animation Source"))
			.ClientSize(FVector2D(500, 600))
			.SupportsMinimize(false).SupportsMaximize(false)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
			[
				ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
			]
			];

		GEditor->EditorAddModalWindow(PickerWindow.ToSharedRef());
		PickerWindow.Reset();
	}
	return TargetAnimSource != nullptr;
}

UObject* UPaperZDAnimMappingPFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (TargetAnimSource && TargetAnimSource->GetSupportedAnimSequenceClass())
	{
		UPaperZDAnimMapping* AnimMapping = NewObject<UPaperZDAnimMapping>(InParent, UPaperZDAnimMapping::StaticClass(), Name, Flags);
		AnimMapping->SetAnimSource(TargetAnimSource);
		return AnimMapping;
	}
	else if (TargetAnimSource->GetSupportedAnimSequenceClass() == nullptr)
	{
		Warn->Logf(ELogVerbosity::Error, TEXT("Could not create Animation, Animation Source has no valid default supported animation class."));
	}

	return nullptr;
}

void UPaperZDAnimMappingPFactory::OnTargetAnimSourceSelected(const FAssetData& SelectedAsset)
{
	TargetAnimSource = Cast<UPaperZDAnimationSource>(SelectedAsset.GetAsset());
	PickerWindow->RequestDestroyWindow();
}

