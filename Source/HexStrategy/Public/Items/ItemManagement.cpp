#include "ItemManagement.h"

bool UItemManagementLibrary::GetNumTotal(FItemManagementData Data, int32& num)
{
	if (!Data.bLimitedNumber)
	{
		num =  Data.ItemHolders.Num();
		return true;
	}

	num = -1;
	return false;
}

bool UItemManagementLibrary::GetNumAvailable(FItemManagementData Data, int32& num)
{
	if (!Data.bLimitedNumber)
	{
		num = Data.GetNumUnheldItems();
		return true;
	}

	num = -1;
	return false;
}

void FItemManagementData::Add(FName ItemHolder)
{
	ItemHolders.Add(ItemHolder);
}

void FItemManagementData::AddMultiple(int32 number)
{
	for (int i = 0; i < number; i++)
	{
		ItemHolders.Add(NAME_None);
	}
}

void FItemManagementData::RemoveMultiple(int32 number)
{
	for (int i = 0; i < number; i++)
	{
		ItemHolders.Remove(NAME_None);
	}
}

int32 FItemManagementData::GetNumUnheldItems()
{
	int32 cnt = 0;
	for (FName Holder : ItemHolders)
	{
		if (Holder == NAME_None)
			cnt++;
	}

	return cnt;
}
