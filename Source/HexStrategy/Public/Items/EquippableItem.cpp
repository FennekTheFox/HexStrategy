#include "EquippableItem.h"

#include "Units/UnitAttributeModifiers.h"
#include "Units/UnitData.h"
#pragma once


void UEquippableItem::AddModifierToUnitIfNeeded(int32 Value, FUnitAttribute& AttributeToModify)
{
	if (Value != 0)
	{
		UUnitAttributeModifier_Add* NewMod = NewObject<UUnitAttributeModifier_Add>(UUnitAttributeModifier_Add::StaticClass());
		NewMod->Summand = Value;
		NewMod->Source = this;

		AttributeToModify.AddModifier(NewMod);
	}
}


void UEquippableItem::RemoveModifierFromUnitIfNeeded(int32 Value, FUnitAttribute& AttributeToModify)
{
	if (Value != 0)
	{
		for (UUnitAttributeModifier* Mod : AttributeToModify.Modifiers)
		{
			if (Mod->Source == this)
			{
				AttributeToModify.RemoveModifier(Mod);
				return;
			}
		}

		ensureMsgf(false, TEXT("Attempted to remove a modifier from UnitAttributes, but no modifier with the correct source could be found."));
	}
}

bool UEquippableItem::UnitFulfillsProficiencyRequirement(UUnitData* Unit)
{
	if (Unit->Attributes.Vigor.GetBaseValue() < ProficiencyRequirements.Vigor) return false;
	if (Unit->Attributes.Ferocity.GetBaseValue() < ProficiencyRequirements.Ferocity) return false;
	if (Unit->Attributes.Agility.GetBaseValue() < ProficiencyRequirements.Agility) return false;
	if (Unit->Attributes.Cunning.GetBaseValue() < ProficiencyRequirements.Cunning) return false;
	if (Unit->Attributes.Endurance.GetBaseValue() < ProficiencyRequirements.Endurance) return false;
	if (Unit->Attributes.Resilience.GetBaseValue() < ProficiencyRequirements.Resilience) return false;
	if (Unit->Attributes.Swiftness.GetBaseValue() < ProficiencyRequirements.Swiftness) return false;

	return true;
}

void UEquippableItem::EquipToUnit_Implementation(UUnitData* Unit)
{
	AddModifierToUnitIfNeeded(AttributeModifiers.Vigor, Unit->Attributes.Vigor);
	AddModifierToUnitIfNeeded(AttributeModifiers.Ferocity, Unit->Attributes.Ferocity);
	AddModifierToUnitIfNeeded(AttributeModifiers.Agility, Unit->Attributes.Agility);
	AddModifierToUnitIfNeeded(AttributeModifiers.Cunning, Unit->Attributes.Cunning);
	AddModifierToUnitIfNeeded(AttributeModifiers.Endurance, Unit->Attributes.Endurance);
	AddModifierToUnitIfNeeded(AttributeModifiers.Resilience, Unit->Attributes.Resilience);
	AddModifierToUnitIfNeeded(AttributeModifiers.Swiftness, Unit->Attributes.Swiftness);

	if (UnitFulfillsProficiencyRequirement(Unit))
	{
		for(auto ProficiencyAbility : ProficiencyAbilities)
			Unit->EquipmentAbilities.Add(ProficiencyAbility);
	}
}

void UEquippableItem::UnequipFromUnit_Implementation(UUnitData* Unit)
{
	RemoveModifierFromUnitIfNeeded(AttributeModifiers.Vigor, Unit->Attributes.Vigor);
	RemoveModifierFromUnitIfNeeded(AttributeModifiers.Ferocity, Unit->Attributes.Ferocity);
	RemoveModifierFromUnitIfNeeded(AttributeModifiers.Agility, Unit->Attributes.Agility);
	RemoveModifierFromUnitIfNeeded(AttributeModifiers.Cunning, Unit->Attributes.Cunning);
	RemoveModifierFromUnitIfNeeded(AttributeModifiers.Endurance, Unit->Attributes.Endurance);
	RemoveModifierFromUnitIfNeeded(AttributeModifiers.Resilience, Unit->Attributes.Resilience);
	RemoveModifierFromUnitIfNeeded(AttributeModifiers.Swiftness, Unit->Attributes.Swiftness);

	if (UnitFulfillsProficiencyRequirement(Unit))
	{
		for (auto ProficiencyAbility : ProficiencyAbilities)
			Unit->EquipmentAbilities.Add(ProficiencyAbility);
	}
}
