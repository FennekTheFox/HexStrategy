#pragma once

#include "Races.h"
#include "UnitAttributes.h"
#include "Affiliation.h"
#include "UnitEquipment.h"
#include "UnitInventory.h"

#include "UnitData.generated.h"


USTRUCT(BlueprintType)
struct FUnitProfessionProgress
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 Level = 1;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 ProfessionExp;
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

	//The specific values used to tweak racial appearance
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Appearance", SaveGame)
		FUnitBodyData UnitBodyData;
	//The class template which this unit uses
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Appearance", SaveGame, meta = (AllowedClasses = "UnitProfession"))
		FSoftObjectPath UnitProfession;

	//The units attributes
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Attributes", SaveGame)
		FUnitAttributeBlock Attributes;

	/*The unit's equiped items*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Items", SaveGame)
		FUnitEquipment UnitEquippedItems;
	/*The unit's carried items*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Items", SaveGame)
		FUnitInvenotry UnitInventory;

	/*The number of experience points earned by this unit*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Progression", SaveGame)
		int32 UnitExperiencePoints;
	/*The list of levels and exp gained for all professions of this unit*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Progression", SaveGame)
		TMap<FName, FUnitProfessionProgress> UnitProfessionProgress;

	/*The Abilities the unit has slotted as its active abilities*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Abilities", SaveGame)
		TArray<FGameplayAbilitySpecDef> SlottedAbilities;
	/*The Abilities the unit has learned*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Abilities", SaveGame)
		TArray<FGameplayAbilitySpecDef> LearnedAbilities;
	/*The list of abilities granted to the unit by its equipment*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Abilities", SaveGame)
		TArray<FGameplayAbilitySpecDef> EquipmentAbilities;



	//The point in time when the unit has been recruited
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Meta", SaveGame)
		FDateTime JoinedDateTime;
	//The point in time when the unit has last been used
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Meta", SaveGame)
		FDateTime LastUsedDateTime;


public:
	UFUNCTION(BlueprintCallable)
		class UUnitData* Duplicate();
	UFUNCTION(BlueprintPure, Category = "UnitData")
		class UUnitRace* GetRace();
	UFUNCTION(BlueprintPure, Category = "UnitData")
		class UBodyTypeAppearanceData* GetAppearanceData();
	UFUNCTION(BlueprintPure, Category = "UnitData")
		class UUnitProfession* GetProfession();

private:
	/*Function to get the possible Super Body Options, defined here since this object
	owns the body data and ustructs cant define ufunctions*/
	UFUNCTION()
		TArray<FName> GetSuperBodyOptions();
	/*Function to get the possible Sub Body Options, defined here since this object
	owns the body data and ustructs cant define ufunctions*/
	UFUNCTION()
		TArray<FName> GetSubBodyOptions();
};