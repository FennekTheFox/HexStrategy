#include "GameFileData.h"

#include "Items/ItemManagement.h"

void UGameFileData::AddItemToPlayerInventory(class UItemBase* ItemToAdd, int32 Amount)
{
	if(!ItemToAdd ||Amount<=0)
		ensureMsgf(false, TEXT("Tried to add an item of invalid class or amount to player inventory."));

	FItemManagementData& DataRef = PlayerInventory.FindOrAdd(ItemToAdd->ItemID);
	DataRef.bLimitedNumber = false;
	DataRef.Item = ItemToAdd;
	DataRef.AddMultiple(Amount);
}

bool UGameFileData::RemoveItemFromPlayerInventory(class UItemBase* ItemToAdd, int32 Amount)
{
	FItemManagementData& DataRef = PlayerInventory.FindOrAdd(ItemToAdd->ItemID);
	if (Amount <= DataRef.GetNumUnheldItems())
	{
		DataRef.RemoveMultiple(Amount);
	}

	return false;
}

bool UGameFileData::DoesPlayerHaveItem(class UItemBase* ItemToAdd, int32 Amount)
{
	ensureMsgf(false, TEXT("This function still needs to be implemented"));
	return false;
}

