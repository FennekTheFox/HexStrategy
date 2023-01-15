#include "GameFileData.h"

#include "Items/ItemManagement.h"
#include "Net/UnrealNetwork.h"

void UGameFileData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UGameFileData, MetaData);
	DOREPLIFETIME(UGameFileData, PlayerUnitCharacter);
	DOREPLIFETIME(UGameFileData, PlayerUnits);
	DOREPLIFETIME(UGameFileData, PlayerLocation_MapName);
	DOREPLIFETIME(UGameFileData, PlayerLocation_PawnLocation);
	DOREPLIFETIME(UGameFileData, PlayerInventory);
	DOREPLIFETIME(UGameFileData, GameplayFlags);
}

void UGameFileData::AddItemToPlayerInventory_Implementation(class UItemBase* ItemToAdd, int32 Amount)
{
	if (!ItemToAdd || Amount <= 0)
		ensureMsgf(false, TEXT("Tried to add an item of invalid class or amount to player inventory."));

	FItemManagementData* DataPtr = PlayerInventory.FindByPredicate(
		[ItemToAdd] (const FItemManagementData& Data) {return Data.Item == ItemToAdd;});

	if (!DataPtr)
	{
		DataPtr = &PlayerInventory.Emplace_GetRef(FItemManagementData());
	}
	
	DataPtr->bLimitedNumber = false;
	DataPtr->Item = ItemToAdd;
	DataPtr->AddMultiple(Amount);
}

void UGameFileData::RemoveItemFromPlayerInventory_Implementation(class UItemBase* ItemToAdd, int32 Amount)
{
	FItemManagementData* DataPtr = PlayerInventory.FindByPredicate(
		[ItemToAdd](const FItemManagementData& Data) {return Data.Item == ItemToAdd;});

	if (DataPtr && Amount <= DataPtr->GetNumUnheldItems())
	{
		DataPtr->RemoveMultiple(Amount);
	}
}

bool UGameFileData::DoesPlayerHaveItem(class UItemBase* ItemToAdd, int32 Amount)
{
	ensureMsgf(false, TEXT("This function still needs to be implemented"));
	return false;
}

void UGameFileData::AddFlag_Implementation(FGameplayTag NewFlag)
{
	if (GameplayFlags.HasTagExact(NewFlag)) return;

	GameplayFlags.AddTag(NewFlag);
}

bool UGameFileData::HasFlag(FGameplayTag Flag)
{
	return GameplayFlags.HasTagExact(Flag);
}

UGameFile_SaveGameWrapper* UGameFileData::SerializeToWrapper()
{
	UGameFile_SaveGameWrapper* Wrapper = NewObject<UGameFile_SaveGameWrapper>();
	GetRecord(this, Wrapper->InternalRecord);
	return Wrapper;
}

void UGameFileData::GetRecord(UGameFileData* Data, FGameFileRecord& OutRecord)
{
	if(!ensure((Data))) return;

	OutRecord.MetaData = Data->MetaData;
	OutRecord.PlayerUnitCharacter = FObjectRecord::SerializeObject(Data->PlayerUnitCharacter);

	for (UUnitData* Data : Data->PlayerUnits)
	{
		OutRecord.PlayerUnits.Add(FObjectRecord::SerializeObject(Data));
	}

	OutRecord.PlayerLocation_MapName = Data->PlayerLocation_MapName;
	OutRecord.PlayerLocation_PawnLocation = Data->PlayerLocation_PawnLocation;
	OutRecord.PlayerInventory = Data->PlayerInventory;
	OutRecord.GameplayFlags = Data->GameplayFlags;
}

UGameFileData* UGameFileData::ExtractFromWrapper(UGameFile_SaveGameWrapper* Wrapper, UObject* Outer)
{
	return FromRecord(Wrapper->InternalRecord, Outer);
}

UGameFileData* UGameFileData::FromRecord(const FGameFileRecord& Record, UObject* Outer)
{
	UGameFileData* Data = NewObject<UGameFileData>(Outer);

	Data->MetaData = Record.MetaData;
	Data->PlayerUnitCharacter = Record.PlayerUnitCharacter.DeserializeObject<UUnitData>(Data);

	for (FObjectRecord Unit : Record.PlayerUnits)
	{
		Data->PlayerUnits.Add(Unit.DeserializeObject<UUnitData>(Data));
	}

	Data->PlayerLocation_MapName = Record.PlayerLocation_MapName;
	Data->PlayerLocation_PawnLocation = Record.PlayerLocation_PawnLocation;
	Data->PlayerInventory = Record.PlayerInventory;
	Data->GameplayFlags = Record.GameplayFlags;

	return Data;
}

