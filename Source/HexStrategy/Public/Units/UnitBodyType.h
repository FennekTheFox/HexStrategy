#pragma once

UENUM(BlueprintType)
enum class EBodyType : uint8
{
	Human_Male_1,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EBodyType, EBodyType::Count);		//Makes the enum iterable
