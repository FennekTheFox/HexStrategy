#pragma once

#include <Kismet/BlueprintFunctionLibrary.h>
#include "Affiliation.generated.h"

UENUM(BlueprintType)
enum class EUnitAffiliationType : uint8
{
	Player,
	Enemy0,
	Enemy1, 
	Enemy2, 
	Enemy3,
	Allied0,
	Allied1,
	Allied2,
	Allied3,
	Neutral0,
	Neutral1,
	Neutral2,
	Neutral3,
};


USTRUCT(BlueprintType)
struct FUnitAffiliation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EUnitAffiliationType UnitAffiliation = EUnitAffiliationType::Neutral0;

	/*The player controler this unit belongs to, only known to the game mode*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(EditConditionHides, EditCondition="UnitAffiliation = Player"))
		APlayerController* OwningPlayerController;
	/*The PC index this unit belongs to, set by game mode*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 OwningControllerIndex = -1;

	bool operator== (const FUnitAffiliation& other) const 
	{
		return (other.UnitAffiliation == UnitAffiliation && other.OwningControllerIndex == OwningControllerIndex);
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
