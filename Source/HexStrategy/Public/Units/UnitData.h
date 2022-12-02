#pragma once

#include "Races.h"

#include "UnitData.generated.h"




//The data class that contains all the information about a unit relating to 
//it's appearance and abilities
UCLASS(BlueprintType, Blueprintable)
class UUnitData : public UDataAsset
{
	GENERATED_BODY()

public:
	//The units full name
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText UnitName;

	//The class template which this unit uses
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UUnitRaceTemplates* UnitRace;
	//The class template which this unit uses
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UUnitProfessionTemplates* UnitProfession;
	//The specific values used to tweak racial appearance
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FRaceAttributes RaceAttributes;

	
};