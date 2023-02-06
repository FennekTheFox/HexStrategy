// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimSequences/PaperZDAnimSequence_Flipbook.h"
#include "PaperZDCustomVersion.h"
#include "Math/UnrealMathUtility.h"



UPaperFlipbook* UPaperZDAnimSequence_Flipbook::GetFlipbook() const
{
	return AnimDataSource[0];
}

UPaperFlipbook* UPaperZDAnimSequence_Flipbook::GetDirectionalFlipbook(FVector2D Direction) const
{
	static const FVector2D TopPosition(0.0f, 1.0f);
	const float Sign = Direction.X != 0.0f ? FMath::Sign(Direction.X) : 1.0f;
	const float AngleRad = FMath::Acos(Direction.GetSafeNormal() | TopPosition) * Sign;
	float CachedDirectionalAngle = FMath::RadiansToDegrees(AngleRad);

	UPaperFlipbook* out = GetAnimationData<UPaperFlipbook*>(CachedDirectionalAngle);

	return out;
}

void UPaperZDAnimSequence_Flipbook::PostLoad()
{
	Super::PostLoad();

	//Backwards compatibility: after the AnimBlueprint rework, the AnimSequences now can support "multi-directional" data sources
	//which are stored on an array that accommodates its size to the number of directions it needs to use.
	//A non-directional sequence uses a size-1 array, thus we need to move the data from the old non-directional variable into the array.
	//The resulting AnimSequence will be non-directional due to the old version having no directional support whatsoever.
	const int32 ZDVersion = GetLinkerCustomVersion(FPaperZDCustomVersion::GUID);
	if (ZDVersion < FPaperZDCustomVersion::AnimBlueprintRework)
	{
		AnimDataSource.SetNum(1);
		AnimDataSource[0] = Flipbook_DEPRECATED;
		bDirectionalSequence = false;
	}
}

void UPaperZDAnimSequence_Flipbook::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	Ar.UsingCustomVersion(FPaperZDCustomVersion::GUID);
}

float UPaperZDAnimSequence_Flipbook::GetTotalDuration() const
{
	const UPaperFlipbook* PrimaryFlipbook = GetPrimaryFlipbook();
	return PrimaryFlipbook ? PrimaryFlipbook->GetTotalDuration() : 0.0f;
}

float UPaperZDAnimSequence_Flipbook::GetFramesPerSecond() const
{
	//Default value is 15 fps
	const UPaperFlipbook* PrimaryFlipbook = GetPrimaryFlipbook();
	return PrimaryFlipbook ? PrimaryFlipbook->GetFramesPerSecond() : 15.0f;
}

bool UPaperZDAnimSequence_Flipbook::IsDataSourceEntrySet(int32 EntryIndex) const
{
	return AnimDataSource.IsValidIndex(EntryIndex) ? AnimDataSource[EntryIndex] != nullptr : false;
}
