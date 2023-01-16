#include "GameFileData.h"

#include "Items/ItemManagement.h"
#include "Net/UnrealNetwork.h"
#include <Engine/ActorChannel.h>
#include <Net/DataBunch.h>

APlayerGameFileData::APlayerGameFileData()
{
	bReplicates = true;
	bIsSpatiallyLoaded = false;
}

bool APlayerGameFileData::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	return true;
}

void APlayerGameFileData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(APlayerGameFileData, MetaData);
	DOREPLIFETIME(APlayerGameFileData, PlayerUnitCharacter);
	DOREPLIFETIME(APlayerGameFileData, PlayerUnits);
	DOREPLIFETIME(APlayerGameFileData, PlayerLocation_MapName);
	DOREPLIFETIME(APlayerGameFileData, PlayerLocation_PawnLocation);
	DOREPLIFETIME(APlayerGameFileData, PlayerInventory);
	DOREPLIFETIME(APlayerGameFileData, GameplayFlags);
}

bool APlayerGameFileData::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{

	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	bWroteSomething |= Channel->ReplicateSubobject(PlayerUnitCharacter, *Bunch, *RepFlags);
	bWroteSomething |= Channel->ReplicateSubobjectList(PlayerUnits, *Bunch, *RepFlags);

	return bWroteSomething;
}

void APlayerGameFileData::AddItemToPlayerInventory_Implementation(class UItemBase* ItemToAdd, int32 Amount)
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

void APlayerGameFileData::RemoveItemFromPlayerInventory_Implementation(class UItemBase* ItemToAdd, int32 Amount)
{
	FItemManagementData* DataPtr = PlayerInventory.FindByPredicate(
		[ItemToAdd](const FItemManagementData& Data) {return Data.Item == ItemToAdd;});

	if (DataPtr && Amount <= DataPtr->GetNumUnheldItems())
	{
		DataPtr->RemoveMultiple(Amount);
	}
}

bool APlayerGameFileData::DoesPlayerHaveItem(class UItemBase* ItemToAdd, int32 Amount)
{
	ensureMsgf(false, TEXT("This function still needs to be implemented"));
	return false;
}

void APlayerGameFileData::AddFlag_Implementation(FGameplayTag NewFlag)
{
	if (GameplayFlags.HasTagExact(NewFlag)) return;

	GameplayFlags.AddTag(NewFlag);
}

bool APlayerGameFileData::HasFlag(FGameplayTag Flag)
{
	return GameplayFlags.HasTagExact(Flag);
}

UGameFile_SaveGameWrapper* APlayerGameFileData::SerializeToWrapper()
{
	UGameFile_SaveGameWrapper* Wrapper = NewObject<UGameFile_SaveGameWrapper>();
	GetRecord(Wrapper->InternalRecord);
	return Wrapper;
}

UGameFile_SaveGameWrapper* UGameFileData_DataAsset::SerializeToWrapper()
{
	UGameFile_SaveGameWrapper* Wrapper = NewObject<UGameFile_SaveGameWrapper>();
	GetRecord(this, Wrapper->InternalRecord);
	return Wrapper;
}

void UGameFileData_DataAsset::GetRecord(UGameFileData_DataAsset* Data, FGameFileRecord& OutRecord)
{
	if(!ensure((Data))) return;

	OutRecord.MetaData = Data->MetaData;
	OutRecord.PlayerUnitCharacter = FObjectRecord::SerializeObject(Data->PlayerUnitCharacter);

	for (UUnitData* UnitData : Data->PlayerUnits)
	{
		OutRecord.PlayerUnits.Add(FObjectRecord::SerializeObject(UnitData));
	}

	OutRecord.PlayerLocation_MapName = Data->PlayerLocation_MapName;
	OutRecord.PlayerLocation_PawnLocation = Data->PlayerLocation_PawnLocation;
	OutRecord.PlayerInventory = Data->PlayerInventory;
	OutRecord.GameplayFlags = Data->GameplayFlags;
}

UGameFileData_DataAsset* UGameFileData_DataAsset::ExtractFromWrapper(UGameFile_SaveGameWrapper* Wrapper, UObject* Outer)
{
	return FromRecord(Wrapper->InternalRecord, Outer);
}

UGameFileData_DataAsset* UGameFileData_DataAsset::FromRecord(const FGameFileRecord& Record, UObject* Outer)
{
	UGameFileData_DataAsset* Data = NewObject<UGameFileData_DataAsset>(Outer);

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

void APlayerGameFileData::InitializeFromRecord(const FGameFileRecord& Record)
{
	UGameFileData_DataAsset* Data = NewObject<UGameFileData_DataAsset>();

	MetaData = Record.MetaData;
	PlayerUnitCharacter = Record.PlayerUnitCharacter.DeserializeObject<UUnitData>(Data);

	for (FObjectRecord Unit : Record.PlayerUnits)
	{
		PlayerUnits.Add(Unit.DeserializeObject<UUnitData>(Data));
	}

	PlayerLocation_MapName = Record.PlayerLocation_MapName;
	PlayerLocation_PawnLocation = Record.PlayerLocation_PawnLocation;
	PlayerInventory = Record.PlayerInventory;
	GameplayFlags = Record.GameplayFlags;
}

void APlayerGameFileData::GetRecord(FGameFileRecord& OutRecord)
{
	OutRecord.MetaData = MetaData;
	OutRecord.PlayerUnitCharacter = FObjectRecord::SerializeObject(PlayerUnitCharacter);

	for (UUnitData* UnitData : PlayerUnits)
	{
		OutRecord.PlayerUnits.Add(FObjectRecord::SerializeObject(UnitData));
	}

	OutRecord.PlayerLocation_MapName = PlayerLocation_MapName;
	OutRecord.PlayerLocation_PawnLocation = PlayerLocation_PawnLocation;
	OutRecord.PlayerInventory = PlayerInventory;
	OutRecord.GameplayFlags = GameplayFlags;
}

void APlayerGameFileData::BeginPlay()
{
	Super::BeginPlay();
}
