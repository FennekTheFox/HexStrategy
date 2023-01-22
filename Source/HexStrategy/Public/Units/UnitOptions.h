#pragma once

#include "GameplayTagContainer.h"

#include "UnitOptions.generated.h"


/*Base class for picking options. can be used in C++ via the OptionHelpers
namespace functions to randomize and filter attributes*/
USTRUCT(BlueprintType)
struct FOption
{
	GENERATED_BODY()

public:
	/*The weight of this appearance trait, think of it as the "likeliness" of it appearing in the phenotype compared to 
	all the other options*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Weight = 1.f;
	/*Used to tag traits, is queried upon generation. For example, certain traits are only exhibited by certain cultures
	You would then tag the trait with the culture, and when generating tell the generator which culture the unit you're
	trying to generate can belong to.*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTagContainer OptionTags;
};


USTRUCT(BlueprintType)
struct FColourOption : public FOption
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor Colour = FLinearColor(.5f, .5f, .5f, .7f);
};

USTRUCT(BlueprintType)
struct FHairStypeOption : public FOption
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UHairStyle* HairStyle;
};
