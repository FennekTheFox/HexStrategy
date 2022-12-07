#pragma once

#include "FlipbookHairStyle.generated.h"

//class that defines a hairstyle
//ATM only a flipbook suit but implemented for future extendability
UCLASS(BlueprintType)
class UHairStyle : public UDataAsset 
{
	GENERATED_BODY()

public:
	//The Flipbook suit used for this hairstyle
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class USpriteFlipbookSuit* Flipbooks;

	//TODO: reference for the portrait data

};