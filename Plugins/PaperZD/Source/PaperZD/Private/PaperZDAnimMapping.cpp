// Copyright 2023, Fennel Fox, Discord @Fennel#7727. Feel free to use this for anything honestly, go nuts.

#include "PaperZDAnimMapping.h"

#include "AnimSequences/PaperZDAnimSequence.h"


void UPaperZDAnimMapping::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Refresh();
}

void UPaperZDAnimMapping::SetAnimSource(UPaperZDAnimationSource* InAnimSource)
{
	AnimSource = InAnimSource;
}

void UPaperZDAnimMapping::SetEnum(UEnum* NewEnum)
{
	Enum = NewEnum;

	Refresh();
}

void UPaperZDAnimMapping::Refresh()
{
	TArray<FName> KeysToRemove;
	for (auto&& KVPair : Mapping)
	{
		if (KVPair.Value && KVPair.Value->GetAnimSource() != AnimSource)
			KVPair.Value = nullptr;

		if (!IsValidKey(KVPair.Key))
		{
			//Found a key that is no longer valid, remove it
			KeysToRemove.Add(KVPair.Key);
		}
	}
	for (FName Key : KeysToRemove)
	{
		Mapping.Remove(Key);
	}
	for (int i = 0; i < Enum->NumEnums(); i++)
	{
		FName Key = Enum->GetNameByIndex(i);
		Mapping.FindOrAdd(Key);
	}
}

bool UPaperZDAnimMapping::IsValidKey(FName Key)
{
	if(!Enum) return false;

	return Enum->GetValueByName(Key) != INDEX_NONE;
}

