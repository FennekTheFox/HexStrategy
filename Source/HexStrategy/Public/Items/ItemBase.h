#pragma once

#include "ItemBase.generated.h"

/*Enum denoting the type of item to make for easy filtering without much casting*/
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Invalid,

	KeyItem,
	Consumable,
	Material, 
	Trash,

	Armament,
	Armour, 
	Accessory,
};


UCLASS(Abstract, BlueprintType)
class UItemBase : public UDataAsset
{
	GENERATED_BODY()

public:
	/*The localizable name of the Item */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FText ItemName;
	/*The unique item identifier*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName ItemID;
	/*The Icon used to represent this item*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* Icon;

	/*The type of the item, set in child class constructor*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EItemType ItemType {EItemType::Invalid};

	/*Can this item be stacked?*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bStackable = false;
	/*If so, how high can it be stacked?*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(EditCondition="bStackable"))
		int32 StackLimit = 10;

	/*The base value of the item, used for price calculations*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 BaseValue = 1;

public:
	UFUNCTION(BlueprintPure)
		int32 GetPrimaryType() { return (int32)ItemType; }
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		int32 GetSecondaryType();
};
