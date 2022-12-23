#pragma once

#include "EquipableItem.h"
#include "Armour.generated.h"


UENUM(BlueprintType)
enum class EArmourType : uint8
{
	Invalid,
	Head,
	Shoulders,
	Chest,
	Arms,
	Legs,
	Feet,
};



/*Base class for all non-weapon equipable items*/
UCLASS(Abstract, BlueprintType)
class UArmour : public UEquipableItem
{
	GENERATED_BODY()


public:
	UArmour()
	{
		ItemType = EItemType::Armour;
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EArmourType ArmourType {EArmourType::Invalid};
};


/*Head Armour*/
UCLASS(BlueprintType)
class UHeadArmour : public UArmour
{
	GENERATED_BODY()

public:
	UHeadArmour()
		: Super()
	{
		ArmourType = EArmourType::Head;
	}
};

/*Shoulder Armour*/
UCLASS(BlueprintType)
class UShoulderArmour : public UArmour
{
	GENERATED_BODY()

public:
	UShoulderArmour()
		: Super()
	{
		ArmourType = EArmourType::Shoulders;
	}
};

/*Chest Armour*/
UCLASS(BlueprintType)
class UChestArmour : public UArmour
{
	GENERATED_BODY()

public:
	UChestArmour()
		: Super()
	{
		ArmourType = EArmourType::Chest;
	}
};

/*Arms Armour*/
UCLASS(BlueprintType)
class UArmsArmour : public UArmour
{
	GENERATED_BODY()

public:
	UArmsArmour()
		: Super()
	{
		ArmourType = EArmourType::Arms;
	}
};

/*Leg Armour*/
UCLASS(BlueprintType)
class ULegArmour : public UArmour
{
	GENERATED_BODY()

public:
	ULegArmour()
		: Super()
	{
		ArmourType = EArmourType::Legs;
	}
};

/*Feet Armour*/
UCLASS(BlueprintType)
class UFeetArmour : public UArmour
{
	GENERATED_BODY()

public:
	UFeetArmour()
		: Super()
	{
		ArmourType = EArmourType::Feet;
	}
};