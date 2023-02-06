// Copyright 2023, Fennel Fox, Discord @Fennel#7727. Feel free to use this for anything honestly, go nuts.

#pragma once
#include "Factories/Factory.h"
#include "PaperZDAnimMapping.h"
#include "PaperZDAnimMappingFactory.generated.h"

class UPaperZDAnimationSource;
class SWindow;
struct FAssetData;

/**
 * Factory for PaperZD Animation Blueprints.
 */
UCLASS()
class UPaperZDAnimMappingPFactory : public UFactory
{
	GENERATED_BODY()


	/* Picked window Ptr */
	TSharedPtr<SWindow> PickerWindow;


private:
	UPROPERTY()
		class UPaperZDAnimationSource* TargetAnimSource;
	UPROPERTY()
		class UEnum* TargetEnum;

public:
	UPaperZDAnimMappingPFactory(const FObjectInitializer& ObjectInitializer);

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface

private:
	/* Called when the user selects an AnimationSource from the asset picker. */
	void OnTargetAnimSourceSelected(const FAssetData& SelectedAsset);
};
