#pragma once

#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "GAS_UnitAbilities.generated.h"

class AGridTile;
class UUnitAbilityVisualizer;
class UCategoryData;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityDelegate, class UGAS_UnitAbility*, Ability);





UCLASS()
class UGAS_UnitAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGAS_UnitAbility(){};

	//Sounds good, doesnt work
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action Base")
	//	void SupplyTileToAbility(class AGridTile* Target);


public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		FText AbilityLabel;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		UCategoryData* AbilityCategory;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		TSubclassOf<UUnitAbilityVisualizer> VisualizerClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		TMap<FName, float> ResourceCost;




	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		bool bCanOnlyBeExecutedByOwner = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		bool bCanOnlyBeExecutedOnTurn = true;

	//UPROPERTY(BlueprintAssignable)
	//	FAbilityDelegate OnAbilityEnded;
};

/*Abstract class used to validate whether a supplied tile is valid or not*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class UTileValidityChecker : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		bool ValidateTile(AGridTile* TargetTile, AActor* TargetActor);
};



UCLASS()
class UGAS_GridTargetingAbility : public UGAS_UnitAbility
{
	GENERATED_BODY()

public:
	UGAS_GridTargetingAbility() {};

	UPROPERTY(BlueprintReadOnly, Instanced, EditAnywhere, Category = "Ability Settings")
		TArray<UTileValidityChecker*> RequiredTiles;
};