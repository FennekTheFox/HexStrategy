#pragma once

#include "Affiliation.generated.h"

UENUM(BlueprintType)
enum class EUnitAffiliation : uint8
{
	Player0,
	Player1,
	Player2,
	Player3,
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