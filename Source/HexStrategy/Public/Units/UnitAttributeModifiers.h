#pragma once

#include "UnitAttributeModifiers.generated.h"


//Base class for Attribute modifiers
UCLASS(BlueprintType)
class UUnitAttributeModifier : public UObject
{
	GENERATED_BODY()

public:
	//Decides in which order the modifiers get applied, higher priority gets applied first
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn))
		int32 Priority;
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn))
		UObject* Source;

public:
	//Function used to (refcursively) calculate the effective attribute value
	UFUNCTION(BlueprintPure)
		virtual int32 Apply(int32 Value) { return -1; };
};


//Modifier that hard sets the value of the  stat
UCLASS(BlueprintType)
class UUnitAttributeModifier_Set : public UUnitAttributeModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn))
		int32 SetValue;
public:
	virtual int32 Apply(int32 InValue)override { return SetValue; };
};


//Modifier that multiplies the value of an attribute by a given factor
UCLASS(BlueprintType)
class UUnitAttributeModifier_Multiply : public UUnitAttributeModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn))
		float Factor;
public:
	virtual int32 Apply(int32 Value)override { return Value * Factor; };
};


//Modifier that multiplies the value of an attribute by a given factor
UCLASS(BlueprintType)
class UUnitAttributeModifier_Add : public UUnitAttributeModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn))
		int32 Summand;
public:
	virtual int32 Apply(int32 Value)override { return Value + Summand; };
};
