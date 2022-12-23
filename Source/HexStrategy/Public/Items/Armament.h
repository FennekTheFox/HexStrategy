#pragma once

#include "EquipableItem.h"
#include "Armament.generated.h"

UENUM(BlueprintType)
enum class EArmamentType : uint8
{
	Invalid,
	MainHandOnly,
	OneHanded,
	OffHandOnly, 
	TwoHanded
};


/*Armament base class*/
UCLASS(Abstract, BlueprintType)
class UArmament : public UEquipableItem
{
	GENERATED_BODY()

public:
	UArmament()
	{
		ItemType = EItemType::Armament;
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EArmamentType ArmamentType {EArmamentType::Invalid};
};

/*Main hand only armaments*/
UCLASS(BlueprintType)
class UMainHandOnlyArmament : public UArmament
{
	GENERATED_BODY()

public:
	UMainHandOnlyArmament()
	: Super()
	{
		ArmamentType = EArmamentType::MainHandOnly;
	}
};

/*One handed arments for either slot*/
UCLASS(BlueprintType)
class UOneHandedArmament : public UArmament
{
	GENERATED_BODY()

public:
	UOneHandedArmament()
		: Super()
	{
		ArmamentType = EArmamentType::OneHanded;
	}
};

/*Off hand only armaments*/
UCLASS(BlueprintType)
class UOffHandOnlyArmament : public UArmament
{
	GENERATED_BODY()

public:
	UOffHandOnlyArmament()
		: Super()
	{
		ArmamentType = EArmamentType::MainHandOnly;
	}
};

/*Two handed armaments*/
UCLASS(BlueprintType)
class UTwoHandedArmament : public UArmament
{
	GENERATED_BODY()

public:
	UTwoHandedArmament()
		: Super()
	{
		ArmamentType = EArmamentType::TwoHanded;
	}
};