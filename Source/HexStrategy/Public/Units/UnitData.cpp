#include "UnitData.h"


void UUnitData::EquipSlotIfNeeded(FItemSlot& Slot)
{
	if (Slot.ItemData)
	{
		UEquipableItem* EqItem = Cast<UEquipableItem>(UnitEquippedItems.MainHandSlot.ItemData);
		if (EqItem)
		{
			EqItem->EquipToUnit(this);
		}
	}
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

