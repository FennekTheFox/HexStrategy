#include "ItemSlotLibrary.h"

#include "ItemBase.h"
#include "ItemSlot.h"


bool UItemSlotFunctionLibrary::TrySetSlotItem(FItemSlot& Slot, class UItemBase* Item)
{
	//Check that the item has the correct type
	if (!ensure(Item->IsA(Slot.SupportedItemTypes)))
	{
		return false;
	}


	Slot.ItemData = Item;

	return true;
}