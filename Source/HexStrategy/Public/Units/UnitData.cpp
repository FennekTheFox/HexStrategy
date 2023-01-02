#include "UnitData.h"


void UUnitData::EquipSlotIfNeeded(FItemSlot& Slot)
{
	if (Slot.ItemData)
	{
		UEquippableItem* EqItem = Cast<UEquippableItem>(UnitEquippedItems.MainHandSlot.ItemData);
		if (EqItem)
		{
			EqItem->EquipToUnit(this);
		}
	}
}


class UUnitRaceTemplates* UUnitData::GetRace()
{
	return Cast<UUnitRaceTemplates>(UnitRace.TryLoad());
}

class UUnitProfession* UUnitData::GetProfession()
{
	return Cast<UUnitProfession>(UnitProfession.TryLoad());
}

void UUnitData::Initialize()
{
	//Equip all items if needed
	EquipSlotIfNeeded(UnitEquippedItems.MainHandSlot);
	EquipSlotIfNeeded(UnitEquippedItems.OffHandSlot);
	EquipSlotIfNeeded(UnitEquippedItems.HeadSlot);
	EquipSlotIfNeeded(UnitEquippedItems.ChestSlot);
	EquipSlotIfNeeded(UnitEquippedItems.ShouldersSlot);
	EquipSlotIfNeeded(UnitEquippedItems.ArmsSlot);
	EquipSlotIfNeeded(UnitEquippedItems.LegsSlot);
	EquipSlotIfNeeded(UnitEquippedItems.FeetSlot);
	EquipSlotIfNeeded(UnitEquippedItems.NecklaceSlot);
	EquipSlotIfNeeded(UnitEquippedItems.WristSlot);
	EquipSlotIfNeeded(UnitEquippedItems.Ring1Slot);
	EquipSlotIfNeeded(UnitEquippedItems.Ring2Slot);
}

