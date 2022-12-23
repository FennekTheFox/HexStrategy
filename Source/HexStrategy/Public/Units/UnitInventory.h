#pragma once

#include "../Items/ItemSlot.h"
#include "UnitInventory.generated.h"

USTRUCT(BlueprintType)
struct FUnitInvenotry
{
	GENERATED_BODY()

public:
	//Bag slots
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot Slot1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot Slot2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot Slot3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot Slot4;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot Slot5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot Slot6;
};