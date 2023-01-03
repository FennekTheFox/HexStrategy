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


class UUnitData* UUnitData::Duplicate()
{
	UUnitData* Unit = DuplicateObject<UUnitData>(this, GetTransientPackage());
	return Unit;
}

class UUnitRace* UUnitData::GetRace()
{
	return Cast<UUnitRace>(UnitBodyData.UnitRace.TryLoad());
}

class UBodyTypeAppearanceData* UUnitData::GetAppearanceData()
{
	UUnitRace* UnitRace = Cast<UUnitRace>(UnitBodyData.UnitRace.TryLoad());

	if(!UnitRace)
		return nullptr;

	return UnitRace->FindAppearanceData(UnitBodyData.SuperBodyTypeID, UnitBodyData.SubBodyTypeID);
}

class UUnitProfession* UUnitData::GetProfession()
{
	return Cast<UUnitProfession>(UnitProfession.TryLoad());
}

TArray<FName> UUnitData::GetSuperBodyOptions()
{
	TArray<FName> Out;


	UUnitRace* UnitRace = Cast<UUnitRace>(UnitBodyData.UnitRace.TryLoad());

	if (!UnitRace)
		return Out;

	for (FSuperBodyTypeOptions& Opt : UnitRace->BodyTypeOptions)
	{
		Out.Add(Opt.OptionID);
	}

	return Out;
}

TArray<FName> UUnitData::GetSubBodyOptions()
{
	TArray<FName> Out;


	UUnitRace* UnitRace = Cast<UUnitRace>(UnitBodyData.UnitRace.TryLoad());

	if (!UnitRace)
		return Out;

	for (FSuperBodyTypeOptions& Opt : UnitRace->BodyTypeOptions)
	{
		if (Opt.OptionID == UnitBodyData.SuperBodyTypeID)
		{
			for (FSubBodyTypeOptions& SubOpt : Opt.Suboptions)
			{
				Out.Add(SubOpt.OptionID);
			}
		}
	}

	return Out;
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

