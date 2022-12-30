#pragma once

#include "Races.h"
#include "UnitAttributes.h"
#include "Affiliation.h"
#include "UnitEquipment.h"
#include "UnitInventory.h"

#include "UnitData.generated.h"

//The instance describing the appearance of a unit portrait
UCLASS(BlueprintType)
class UPortraitData : public UDataAsset
{
	GENERATED_BODY()

public:
	//The base texture for the portrait. This contains the basic pose
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* BaseTexture;
	//The hair style to be layered onto the base texture
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* HairStyleTexture;
	//The attire to be layered o top of the base texture
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* AttireTextures;
	//The the eyes to be used on the portrait		
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* PotraitEyes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* SkinMask;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* HairMask;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* EyeWhitesMask;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UTexture2D* PupilsMask;


};

//The data class that contains all the information about a unit relating to 
//it's appearance and abilities
UCLASS(BlueprintType, Blueprintable)
class UUnitData : public UDataAsset
{
	GENERATED_BODY()

public:
	void Initialize();
	void EquipSlotIfNeeded(FItemSlot& Slot);

public:
	//The units full name
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
		FText UnitName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
		FName UnitID;

	//The class template which this unit uses
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Appearance")
		class UUnitRaceTemplates* UnitRace;
	//The class template which this unit uses
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Appearance")
		class UUnitProfession* UnitProfession;
	//The specific values used to tweak racial appearance
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Appearance")
		FRaceAttributes RaceAttributes;

	//The units attributes
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Attributes")
		FUnitAttributeBlock Attributes;

	/*The unit's equiped items*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Items")
		FUnitEquipment UnitEquippedItems;
	/*The unit's carried items*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Items")
		FUnitInvenotry UnitInventory;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Items")
		TArray<FGameplayAbilitySpec> SlottedAbilities;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Items")
		TArray<FGameplayAbilitySpec> LearnedAbilities;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Items")
		TArray<FGameplayAbilitySpec> EquipmentAbilities;


	//The point in time when the unit has been recruited
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Meta")
		FDateTime JoinedDateTime;
	//The point in time when the unit has last been used
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Meta")
		FDateTime LastUsedDateTime;
};