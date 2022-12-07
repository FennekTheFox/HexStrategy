#pragma once

#include "Races.h"
#include "UnitAttributes.h"

#include "UnitData.generated.h"




//The data class that contains all the information about a unit relating to 
//it's appearance and abilities
UCLASS(BlueprintType, Blueprintable)
class UUnitData : public UDataAsset
{
	GENERATED_BODY()

public:
	//The units full name
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
		FText UnitName;

	//The class template which this unit uses
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Appearance")
		class UUnitRaceTemplates* UnitRace;
	//The class template which this unit uses
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Appearance")
		class UUnitProfessionTemplates* UnitProfession;
	//The specific values used to tweak racial appearance
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Appearance")
		FRaceAttributes RaceAttributes;

	//The units attributes
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Attributes")
		FUnitAttributeBlock Attributes;


	//The point in time when the unit has been recruited
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Meta")
		FDateTime JoinedDateTime;
	//The point in time when the unit has last been used
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Meta")
		FDateTime LastUsedDateTime;
};