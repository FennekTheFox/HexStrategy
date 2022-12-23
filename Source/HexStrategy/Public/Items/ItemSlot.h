#pragma once

#include "ItemBase.h"
#include "ItemSlot.generated.h"



USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

public:
	FItemSlot()
	{}


	FItemSlot(UClass* InSupportedClass)
	:SupportedItemTypes(InSupportedClass)
	{}

	FItemSlot& operator = (const FItemSlot& other)
	{
		SupportedItemTypes = other.SupportedItemTypes;
		ItemData = other.ItemData;
		Amount = other.Amount;

		return *this;
	}

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TSubclassOf<UItemBase> SupportedItemTypes {UItemBase::StaticClass()};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UItemBase* ItemData;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 Amount;
};
