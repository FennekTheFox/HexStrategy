#pragma once

#include "UnitBodyType.h"
#include "FlipbookSuits.h"
#include "GameplayAbilitySpec.h"
#include "UnitAttributes.h"

#include "Professions.generated.h"



UCLASS(BlueprintType, Blueprintable)
class UUnitProfession : public UDataAsset
{
	GENERATED_BODY()

	UUnitProfession()
	{
		Refresh();
	}

public:
	/*Localizable name of this profession.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession")
		FText ProfessionLabel;
	/*Serializable ID of this profession.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession")
		FName ProfessionID;
	/*The profession's icon.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession")
		UTexture2D* ProfessionIcon;

	/*Flag that indicates whether or not this profession requires any other professions to be leveled*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession|Setup", meta = (ClampMin = 10, ClampMax = 50))
		int32 MaxProfessionLevel;
	/*The list of abilities this profession grants. Upon leveling the profession a unit is granted the ability with the corresponding index.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession|Setup")
		TArray<FGameplayAbilitySpecDef> AbilityList;
	/*Flag to denote base professions that dont require other professions*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession|Setup")
		bool bIsBaseProfession;
	/*List of professions required to level this one.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession|Setup", meta = (EditCategory="bIsBaseProfession"))
		TMap<class UUnitProfession*, int32> ProfessionRequirements;

	/*The Attribute boni granted to the unit with this profession*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession|Attributes")
		FFlatAttributeList ProfessionAttributeBonus;
	/*The growth rate boni granted to the unit with this profession*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession|Attributes")
		FScalarAttributeList ProfessionGrowthRateBonus;

	/*The Attire Flipbook mapping that this profession uses*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession|Appearance")
		TMap<EBodyType, USpriteFlipbookSuit*> ProfessionFlipbooks;
	/*Whether or not the attire will hide the hair layer*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Profession|Appearance")
		bool bEquipmentHidesHair = false;



protected:
	//Makes sure all keys and subkeys are always present
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override
	{
		Refresh();
	}

private:
	void Refresh()
	{
		for (EBodyType BT : TEnumRange<EBodyType>())
		{
			ProfessionFlipbooks.FindOrAdd(BT);
		}
		AbilityList.SetNum(MaxProfessionLevel);

	}

	//... stuff like skin requirements here

};
