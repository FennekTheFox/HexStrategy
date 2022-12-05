#pragma once

#include "Races.generated.h"


UENUM(BlueprintType)
enum class EBodyType : uint8
{
	Humanoid,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EBodyType, EBodyType::Count);		//Makes the enum iterable

USTRUCT(BlueprintType)
struct FRaceAttributes
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLinearColor BodyColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLinearColor EyeColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLinearColor HairColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UFlipbookHairStyle* HairStyle;
};


UCLASS(BlueprintType, Blueprintable)
class UUnitRaceTemplates : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText RaceLabel;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EBodyType RaceBodyType = EBodyType::Humanoid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USpriteFlipbookSuit* BodyFlipbookSuit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FLinearColor, float> PossibleBodyColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FLinearColor, float> PossibleEyeColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FLinearColor, float> PossibleHairColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<class UFlipbookHairStyle*, float> PossibleHairStyles;


protected:

	//... stuff like skin and eye colour range and the 
};
