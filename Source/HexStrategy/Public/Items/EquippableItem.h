#pragma once

#include "ItemBase.h"
#include "Units/UnitAttributes.h"

#include  "EquipableItem.generated.h"




UCLASS(Abstract, BlueprintType)
class UEquipableItem : public UItemBase
{
	GENERATED_BODY()

public:
	//ProficiencyRequirements
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FFlatAttributeList ProficiencyRequirements;
	//ProficiencyBoni
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Items")
		TArray<FGameplayAbilitySpec> ProficiencyAbilities;


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EquipToUnit(UUnitData* Unit);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UnequipFromUnit(UUnitData* Unit);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FFlatAttributeList AttributeModifiers;
private:
	void AddModifierToUnitIfNeeded(int32 Value, FUnitAttribute& AttributeToModify);
	void RemoveModifierFromUnitIfNeeded(int32 Value, FUnitAttribute& AttributeToModify);
	bool UnitFulfillsProficiencyRequirement(UUnitData* Unit);
};