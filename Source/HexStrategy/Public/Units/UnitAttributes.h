#pragma once

#include "UnitAttributeModifiers.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnitAttributes.generated.h"

//UENUM(BlueprintType)
//enum class EAttributeType : uint8
//{
//	Vitality,	//Health
//	Ferocity,	//physical Attack
//	Agility,	//also physical attack, but different
//	Cunning,	//magical attack
//	Stability,	//Physical defense
//	Resilience, //Magical defense
//	Swiftness	//Speeeeed
//};

USTRUCT(BlueprintType, Blueprintable)
struct FUnitAttribute
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 BaseValue = 10.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float GrowthRate = 1.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 MaxValue = 100.f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<UUnitAttributeModifier*> Modifiers;

public:
	void AddModifier(UUnitAttributeModifier* NewModifier)
	{
		Modifiers.AddUnique(NewModifier);

		//TODO: maybe theres a better way? Inserting the modifier at the right place
		//rather than sorting the whole array? Though modifier arrays will be pretty small
		//anyway
		Algo::Sort(Modifiers, [](UUnitAttributeModifier* A, UUnitAttributeModifier* B) {
			return A->Priority < B->Priority;
			}
		);
	}

	void RemoveModifier(UUnitAttributeModifier* NewModifier)
	{
		Modifiers.Remove(NewModifier);
		//The array should still be sorted with an entry gone
	}

	int32 GetEffectiveValue() const
	{
		int32 ret = BaseValue;
		//Iteratively apply all the modifiers
		for (UUnitAttributeModifier* Mod : Modifiers)
		{
			if(ensure(Mod))
				ret = Mod->Apply(ret);
		}

		return ret;
	}
};

//exposes the FUnitAttribute functions to blueprint. USTRUCT functions when?
UCLASS(BlueprintType)
class UUnitAttributeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Unit|Attributes")
		static void AddModifier(UPARAM(ref) FUnitAttribute& Attribute, UUnitAttributeModifier* NewModifier)
	{
		Attribute.AddModifier(NewModifier);
	}

	UFUNCTION(BlueprintCallable, Category = "Unit|Attributes")
		static void RemoveModifier(UPARAM(ref) FUnitAttribute& Attribute, UUnitAttributeModifier* NewModifier)
	{
		Attribute.RemoveModifier(NewModifier);
	}


	UFUNCTION(BlueprintPure, Category = "Unit|Attributes")
		static int32 GetEffectiveValue(UPARAM(ref) FUnitAttribute& Attribute)
	{
		return Attribute.GetEffectiveValue();
	}
};

USTRUCT(BlueprintType)
struct FUnitAttributeBlock
{
	GENERATED_BODY()

public:
	//The unit attributes need to be editanywhere so that premade unit data assets 
	//can be configured
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FUnitAttribute Vitality;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FUnitAttribute Ferocity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FUnitAttribute Agility;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FUnitAttribute Cunning;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FUnitAttribute Stability;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FUnitAttribute Resilience;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FUnitAttribute Swiftness;
	//Luck? RNG professions might be fun
};
