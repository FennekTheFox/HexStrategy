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

UGameFileRecord* UGameFileData::SerializeToRecord()
{	
	UGameFileRecord* Rec = NewObject<UGameFileRecord>();

	Rec->MetaData = MetaData;
	Rec->PlayerUnitCharacter = FObjectRecord::SerializeObject(PlayerUnitCharacter);
	
	for (UUnitData* Data : PlayerUnits)
	{
		Rec->PlayerUnits.Add(FObjectRecord::SerializeObject(Data));
	}

	Rec->PlayerLocation_MapName = PlayerLocation_MapName;
	Rec->PlayerLocation_PawnLocation = PlayerLocation_PawnLocation;
	Rec->PlayerInventory = PlayerInventory;

	return Rec;
}

UGameFileData* UGameFileData::DeserializeFromRecord(UGameFileRecord* Record)
{
	UGameFileData* Data = NewObject<UGameFileData>();

	Data->MetaData = Record->MetaData;
	Data->PlayerUnitCharacter = Record->PlayerUnitCharacter.DeserializeObject<UUnitData>(Data);

	for (FObjectRecord Unit : Record->PlayerUnits)
	{
		Data->PlayerUnits.Add(Unit.DeserializeObject<UUnitData>(Data));
	}

	Data->PlayerLocation_MapName = Record->PlayerLocation_MapName;
	Data->PlayerLocation_PawnLocation = Record->PlayerLocation_PawnLocation;
	Data->PlayerInventory = Record->PlayerInventory;

	return Data;
}

