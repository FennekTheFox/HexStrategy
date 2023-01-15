#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemBase.h"
#include "ItemManagement.generated.h"


/*Struct containing the information for managing all the instances of an item in a given context*/
USTRUCT(BlueprintType)
struct FItemManagementData
{
	GENERATED_BODY()

public:
	/*The item we're trying to manage*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UItemBase* Item;
	/*Whether or not*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bLimitedNumber = true;

	/*Array for the holder of each item's instance. NAME_None = Player inventory, else its the UnitID*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FName> ItemHolders;

public:
	/*Adds an item with specifiable holder*/
	void Add(FName ItemHolder);
	/*Adds a number of items without a specifiable holder*/
	void AddMultiple(int32 number);
	/*Adds a number of items without a specifiable holder*/
	void RemoveMultiple(int32 number);
	/*Returns the number of item instances without a holder*/
	int32 GetNumUnheldItems();
};



UCLASS(BlueprintType)
class UItemManagementLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ItemManagement")
		static bool GetNumTotal(UPARAM(ref)FItemManagementData Data, int32& num);
	UFUNCTION(BlueprintPure, Category = "ItemManagement")
		static bool GetNumAvailable(UPARAM(ref)FItemManagementData Data, int32& num);
};
