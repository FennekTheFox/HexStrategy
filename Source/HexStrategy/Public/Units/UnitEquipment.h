#pragma once

#include "Items/ItemSlot.h"
#include "Items/Armament.h"
#include "Items/Armour.h"
#include "Items/Accessories.h"
#include "UnitEquipment.generated.h"



USTRUCT(BlueprintType)
struct FUnitEquipment
{
	GENERATED_BODY()

	FUnitEquipment(){};

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot MainHandSlot{ UArmament::StaticClass() };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot OffHandSlot = { UArmament::StaticClass() };


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot HeadSlot = { UHeadArmour::StaticClass() };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot ChestSlot = { UChestArmour::StaticClass() };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot ShouldersSlot = { UShoulderArmour::StaticClass() };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot ArmsSlot = { UArmsArmour::StaticClass() };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot LegsSlot = { ULegArmour::StaticClass() };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot FeetSlot = { UFeetArmour::StaticClass() };


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot NecklaceSlot = { UNecklaceAccessory::StaticClass() };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot WristSlot = { UWristAccessory::StaticClass() };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot Ring1Slot = { URingAccessory::StaticClass() };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FItemSlot Ring2Slot = { URingAccessory::StaticClass() };
};