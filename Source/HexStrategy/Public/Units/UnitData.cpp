#include "UnitData.h"

#include <Net/UnrealNetwork.h>


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


void UUnitData::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, UnitName, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, UnitID, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, UnitBodyData, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, UnitProfession, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, Attributes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, UnitEquippedItems, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, UnitInventory, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, UnitExperiencePoints, COND_None, REPNOTIFY_Always);
	//DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, UnitProfessionProgress, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, SlottedAbilities, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, LearnedAbilities, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, EquipmentAbilities, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, JoinedDateTime, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, LastUsedDateTime, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUnitData, Faction, COND_None, REPNOTIFY_Always);
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

UUnitData::UUnitData()
{
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

