#pragma once

#include "Factions.generated.h"



/*An asset type that denotes the basic properties of an affiliation*/
UCLASS(BlueprintType)
class UUnitFactionData : public UDataAsset
{
	GENERATED_BODY()

public:
	/*The affiliations display name*/
	UPROPERTY(BlueprintReadWrite, Editanywhere, Replicated, Category = "Affiliation")
		FText Affiliation_DisplayName;
	/*The affiliation's ID*/
	UPROPERTY(BlueprintReadWrite, Editanywhere, Replicated, Category = "Affiliation")
		FName Affiliation_ID;

	/*If true, this color will always be the display color for this faction*/
	UPROPERTY(BlueprintReadWrite, Editanywhere, Replicated, Category = "Affiliation|Color")
		bool bUseAffiliationColorOverride;
	UPROPERTY(BlueprintReadWrite, Editanywhere, Replicated, Category = "Affiliation|Color", meta = (EditCondition = "bUseAffiliationColorOverride", EditConditionHides))
		FLinearColor ColorOverride;

	//IDEA: faction banners & stuff

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

};


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
struct FRelationMap
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FName, EFactionRelationState> RelationStates;
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