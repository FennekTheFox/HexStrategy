#pragma once

#include "EquippableItem.h"
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


USTRUCT(BlueprintType)
struct FAttributeScaling
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.f, ClamMax = 2.f))
		float VigorScaling = 0.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.f, ClamMax = 2.f))
		float FerocityScaling = 0.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.f, ClamMax = 2.f))
		float AgilityScaling = 0.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.f, ClamMax = 2.f))
		float CunningScaling = 0.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.f, ClamMax = 2.f))
		float EnduranceScaling = 0.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.f, ClamMax = 2.f))
		float ResilienceScaling = 0.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.f, ClamMax = 2.f))
		float SwiftnessScaling = 0.f;
};


/*Armament base class*/
UCLASS(Abstract, BlueprintType)
class UArmament : public UEquippableItem
{
	GENERATED_BODY()

public:
	UArmament()
	{
		ItemType = EItemType::Armament;
	}

	//The type of armament, inherited by the parent class's constructor
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EArmamentType ArmamentType {EArmamentType::Invalid};

	//Attribute scalings
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FAttributeScaling AttributeScaling;

	void EquipToUnit_Implementation(UUnitData* Unit) override;
	void UnequipFromUnit_Implementation(UUnitData* Unit) override;

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