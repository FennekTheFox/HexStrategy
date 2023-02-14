#pragma once

#include "Factions.generated.h"

/*
Enum denoting the relation between factions, used for gameplay behavior
Neutral		- These factions will neither attack nor aid each other
Enemy		- These units will attack eachother
Allied		- These units will aid each other
*/
UENUM(BlueprintType)
enum class EFactionRelationState : uint8
{
	Neutral,
	Enemy,
	Allied
};

USTRUCT(BlueprintType)
struct FFactionRelationship
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UUnitFactionData* TargetFaction;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		EFactionRelationState RelationState;
};


/*An asset type that denotes the basic properties of an affiliation*/
UCLASS(BlueprintType)
class UUnitFactionData : public UDataAsset
{
	GENERATED_BODY()

public:
	/*The affiliations display name*/
	UPROPERTY(BlueprintReadWrite, Editanywhere, Replicated, Category = "Affiliation")
		FText Faction_DisplayName;
	/*The affiliation's ID*/
	UPROPERTY(BlueprintReadWrite, Editanywhere, Replicated, Category = "Affiliation")
		FName Faction_ID;


	UPROPERTY(BlueprintReadWrite, Editanywhere, Replicated, Category = "Affiliation")
		TArray<FFactionRelationship>  Relations;

	//IDEA: faction banners & stuff
	/*This color will always be the display color for this faction*/
	UPROPERTY(BlueprintReadWrite, Editanywhere, Replicated, Category = "Affiliation|Color")
		FLinearColor ColorOverride;

	
public:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

public:
	UFUNCTION(BlueprintPure)
		EFactionRelationState GetRelationTo(const UUnitFactionData* OtherFaction);

};


/*An asset type that denotes the relationship between all affiliations*/
UCLASS(BlueprintType)
class UUnitFactionRelations : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Editanywhere, Category = "Relations")
		TMap<FName, EFactionRelationState> Relations;
};