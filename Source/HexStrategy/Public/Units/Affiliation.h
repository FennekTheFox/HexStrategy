#pragma once

#include <Kismet/BlueprintFunctionLibrary.h>
#include "Affiliation.generated.h"

UENUM(BlueprintType)
enum class EUnitAffiliationType : uint8
{
	Player,
	Enemy,
	Allied,
	Neutral
};


USTRUCT(BlueprintType)
struct FUnitAffiliation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EUnitAffiliationType UnitAffiliation = EUnitAffiliationType::Neutral;

	/*The player controler this unit belongs to, only known to the game mode*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(EditConditionHides, EditCondition="UnitAffiliation = Player"))
		APlayerController* OwningPlayerController;
	/*The PC index this unit belongs to, set by game mode*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 AffiliationIndex = -1;	


	bool operator== (const FUnitAffiliation& other) const 
	{
		return (other.UnitAffiliation == UnitAffiliation && other.AffiliationIndex == AffiliationIndex);
	}
};


UCLASS()
class UAffiliationBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Affiliation)", CompactNodeTitle = "==", BlueprintThreadSafe), Category = "Affiliation")
		static bool EqualEqual_UnitAffiliation(FUnitAffiliation A, FUnitAffiliation B)
	{
		return A == B;
	}
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Affiliation)", CompactNodeTitle = "==", BlueprintThreadSafe), Category = "Affiliation")
		static bool NotEqual_UnitAffiliation(FUnitAffiliation A, FUnitAffiliation B)
	{
		return !(A == B);
	}
};

