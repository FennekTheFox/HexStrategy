#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ItemSlotLibrary.generated.h"


UCLASS(BlueprintType)
class UItemSlotFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	* Tries to set the item in the slot, checks for supported type
	*/
	UFUNCTION(BlueprintCallable)
		static bool TrySetSlotItem(UPARAM(ref) FItemSlot& Slot, class UItemBase* Item);
};
