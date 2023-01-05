#pragma once

#include "Affiliation.generated.h"

UENUM(BlueprintType)
enum class EUnitAffiliation : uint8
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
		EUnitAffiliation UnitAffiliation = EUnitAffiliation::Neutral0;

	/*The player controler this unit belongs to, only known to the game mode*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(EditConditionHides, EditCondition="UnitAffiliation = Player"))
		APlayerController* OwningPlayerController;
	/*The PC index this unit belongs to, set by game mode*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 OwningPlayerIndex = -1;
};