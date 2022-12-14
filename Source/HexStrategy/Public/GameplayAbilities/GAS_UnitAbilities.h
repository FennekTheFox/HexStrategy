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
		bool bCanOnlyBeExecutedByOwner = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		bool bCanOnlyBeExecutedOnTurn = true;

	//UPROPERTY(BlueprintAssignable)
	//	FAbilityDelegate OnAbilityEnded;
};