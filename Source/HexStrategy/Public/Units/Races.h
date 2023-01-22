#pragma once

#include "UnitPortraits.h"
#include "UnitBodyType.h"
#include "UnitOptions.h"

#include "Races.generated.h"


/*Struct outlining the appearance of a unit
In combination with the pointer to the race data asset this should be enough to
deduce both the portrait and the flipbook data*/
USTRUCT(BlueprintType)
struct FUnitBodyData
{
	GENERATED_BODY()

public:
	//The class template which this unit uses
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Appearance", SaveGame, meta = (AllowedClasses = "UnitRace"))
		FSoftObjectPath UnitRace;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (GetOptions = "GetSuperBodyOptions"))
		FName SuperBodyTypeID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (GetOptions = "GetSubBodyOptions"))
		FName SubBodyTypeID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLinearColor BodyColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLinearColor EyeColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLinearColor HairColor;
	//The chosen HairStyle for this unit
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UHairStyle* HairStyle;

};

/*Struct describing the possible body types a race can have*/
USTRUCT(BlueprintType)
struct FSubBodyTypeOptions
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FText OptionDisplayName;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName OptionID;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UBodyTypeAppearanceData* Appearance;
};

/*Struct describing the possible body types a race can have*/
USTRUCT(BlueprintType)
struct FSuperBodyTypeOptions
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FText OptionDisplayName;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName OptionID;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FSubBodyTypeOptions> Suboptions;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTagContainer SuperBodyTypeTags;
};


/*Data asset describing the basic characteristics a race can have, both
gameplay wise and in terms of appearances*/
UCLASS(BlueprintType)
class UUnitRace : public UDataAsset
{
	GENERATED_BODY()

public:
	//The localizable name of the race
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText RaceLabel;
	//The body type flipbook suit for the race
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance")
		TArray<FSuperBodyTypeOptions> BodyTypeOptions;


	//The localizable name of the race
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		FFlatAttributeList RacialAttributeBonus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		TArray<FGameplayAbilitySpecDef> RacialAbilities;

public:
	UFUNCTION(BlueprintPure)
		UBodyTypeAppearanceData* FindAppearanceData(FName SuperBodyType, FName SubBodyType);
	UFUNCTION(BlueprintPure)
		FName GetSuperBodyTypeIDForAppearance(UBodyTypeAppearanceData* Appearance);
	UFUNCTION(BlueprintPure)
		FName GetSubBodyTypeIDForAppearance(UBodyTypeAppearanceData* Appearance);
};


//This data asset class is used to define the possible appearance and ability values that a 
//Race can have.
UCLASS(BlueprintType, Blueprintable)
class UBodyTypeAppearanceData : public UDataAsset
{
	GENERATED_BODY()

public:
	/*The sprite flipbook suit used for this appearance data*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USpriteFlipbookSuit* BodyFlipbookSuit;
	//The chosen portrait data for this object
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UPortraitData* PortraitData;

	//The body type to be of the race 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EBodyType RaceBodyType = EBodyType::Humanoid;

	//Weighted array to define possible body colors
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FColourOption> PossibleBodyColors;
	//Weighted array to define possible Eye colors
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FColourOption> PossibleEyeColors;
	//Weighted array to define possible hair colors
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FColourOption> PossibleHairColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FColourOption> PossiblePortraitEyes;
	//The number and distribution for all possible hair styles
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FHairStypeOption> PossibleHairStyles;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FGameplayTagContainer BodyTypeTags;

};
