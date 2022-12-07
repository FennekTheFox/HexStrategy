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
		class UHairStyle* HairStyle;
};

//This data asset class is used to define the possible appearance and ability values that a 
//Race can have.
UCLASS(BlueprintType, Blueprintable)
class UUnitRaceTemplates : public UDataAsset
{
	GENERATED_BODY()

public:
	//The localizable name of the race
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText RaceLabel;

	//The body type to be of the race 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EBodyType RaceBodyType = EBodyType::Humanoid;

	//The body type flipbook suit for the race
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USpriteFlipbookSuit* BodyFlipbookSuit;
	//Weighted array to define possible body colors
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FLinearColor, float> PossibleBodyColors;
	//Weighted array to define possible Eye colors
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FLinearColor, float> PossibleEyeColors;
	//Weighted array to define possible hair colors
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FLinearColor, float> PossibleHairColors;
	//The number and distribution for all possible hair styles
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<class UHairStyle*, float> PossibleHairStyles;

	//TODO: 
};
