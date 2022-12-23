#pragma once

#include "EquipableItem.h"
#include "Accessories.generated.h"

UENUM(BlueprintType)
enum class EAccessoryType : uint8
{
	Invalid,
	Necklace,
	Wrist,
	Ring
};

/*Base class for all non-weapon equipable items*/
UCLASS(Abstract, BlueprintType)
class UAccessory : public UEquipableItem
{
	GENERATED_BODY()


public:
	UAccessory()
	{
		ItemType = EItemType::Accessory;
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EAccessoryType AccessoryType {EAccessoryType::Invalid};
};



/*Neck Accessory*/
UCLASS(BlueprintType)
class UNecklaceAccessory : public UAccessory
{
	GENERATED_BODY()

public:
	UNecklaceAccessory()
		: Super()
	{
		AccessoryType = EAccessoryType::Necklace;
	}
};

/*Wrist Accessory*/
UCLASS(BlueprintType)
class UWristAccessory : public UAccessory
{
	GENERATED_BODY()

public:
	UWristAccessory()
		: Super()
	{
		AccessoryType = EAccessoryType::Wrist;
	}
};

/*Ring Accessory*/
UCLASS(BlueprintType)
class URingAccessory : public UAccessory
{
	GENERATED_BODY()

public:
	URingAccessory()
		: Super()
	{
		AccessoryType = EAccessoryType::Ring;
	}
};