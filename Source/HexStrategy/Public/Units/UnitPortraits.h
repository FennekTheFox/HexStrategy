#pragma once

#include "Professions.h"
#include "UnitPortraits.generated.h"

UENUM(BlueprintType)
enum class EBaseEmotion : uint8
{
	Neutral,

	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EBaseEmotion, EBaseEmotion::Count);		//Makes the enum iterable

USTRUCT(BlueprintType)
struct FEyePortraitData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* BaseColour;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* PupilColourTintMask;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* EyebrowTintMask;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* EyeWhitesTintMask;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* Outlines;
};


USTRUCT(BlueprintType)
struct FAttirePortraitData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* BaseColor;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* TintMask;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* Outlines;
};

USTRUCT(BlueprintType)
struct FHairStylePortraitData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* BaseColor;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* TintMask;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* Outlines;
};

//The instance describing the appearance of a unit portrait
UCLASS(BlueprintType)
class UPortraitData : public UDataAsset
{
	GENERATED_BODY()

public:
	//The body colour texture
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* BodyColourTexture;
	//The outlines for the body texture
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* BodyTextureOutlines;


	//The attire to be layered o top of the base texture
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TMap<TSubclassOf<UUnitProfession>, FAttirePortraitData> AttireTextures;

	//The hair style to be layered onto the base texture
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TMap<FName, FHairStylePortraitData> HairStyleTexture;

	//Facial Data
	//The the eyes to be used on the portrait	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<EBaseEmotion, FEyePortraitData> PotraitEyes;
	//The the eyes to be used on the portrait		
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<EBaseEmotion, UTexture2D*> PotraitMouthes;

	////UPROPERTY(BlueprintReadOnly, EditAnywhere)
	////	UTexture2D* SkinMask;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
	//	UTexture2D* HairMask;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
	//	UTexture2D* EyeWhitesMask;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
	//	UTexture2D* PupilsMask;


	//Makes sure all keys and subkeys are always present
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override
	{
		Refresh();
	}

	void Refresh()
	{
		for (EBaseEmotion Dir : TEnumRange<EBaseEmotion>())
		{
			PotraitEyes.FindOrAdd(Dir);
			PotraitMouthes.FindOrAdd(Dir);
		}
	}

};