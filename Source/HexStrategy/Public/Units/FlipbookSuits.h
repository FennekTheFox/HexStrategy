#pragma once

#include "PaperFlipbook.h"

#include <Engine/DataAsset.h>

#include "FlipbookSuits.generated.h"

//Enum listing all the possible facing directions of a unit
UENUM(BlueprintType)
enum class EFacingDirection : uint8
{
	Front = 0,
	FrontLeft = 1,
	FrontRight = 2,
	Left = 3,
	Right = 4,
	BackLeft = 5,
	BackRight = 6,
	Back = 7,


	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EFacingDirection, EFacingDirection::Count);		//Makes the enum iterable



//Enum listing all the possible sprite animation states
UENUM(BlueprintType)
enum class ESpriteAnimationState : uint8
{
	Idle = 0,
	Walk = 1,
	Jump = 2,
	MeleeAttack = 3,
	RangedAttack = 4,
	MagicAttack = 5,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ESpriteAnimationState, ESpriteAnimationState::Count);		//Makes the enum iterable


//A struct that contains a number of flipbooks for all possible facing directions
USTRUCT(BlueprintType)
struct FDirectionalFlipbookSuit
{
	GENERATED_BODY()

		FDirectionalFlipbookSuit()
	{
		Refresh();
	}

	void Refresh()
	{
		for (EFacingDirection Dir : TEnumRange<EFacingDirection>())
		{
			DirectionalFlipbooks.FindOrAdd(Dir);
		}
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<EFacingDirection, UPaperFlipbook*> DirectionalFlipbooks;
};


//A struct that contains a number of flipbooks for all possible facing directions
UCLASS(BlueprintType)
class USpriteFlipbookSuit : public UDataAsset
{
	GENERATED_BODY()

		USpriteFlipbookSuit()
	{
		Refresh();
	}

	void Refresh()
	{
		for (ESpriteAnimationState AS : TEnumRange<ESpriteAnimationState>())
		{
			FDirectionalFlipbookSuit& Suit = FlipbookSuits.FindOrAdd(AS);
			Suit.Refresh();
		}
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<ESpriteAnimationState, FDirectionalFlipbookSuit> FlipbookSuits;

protected:
	//Makes sure all keys and subkeys are always present
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override
	{
		Refresh();
	}
};