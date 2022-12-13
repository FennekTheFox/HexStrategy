#pragma once

#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "GAS_UnitAbilities.generated.h"

class AGridTile;


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityDelegate, class UGAS_UnitAbility*, Ability);


//General category data asset. Defines categories to have
//A localizable lable, tag and priority
UCLASS(BlueprintType)
class UCategoryData : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CategorySettings")
		FText CategoryLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CategorySettings")
		FName CategoryName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CategorySettings")
		int32 CategoryPriority;
};

UCLASS()
class UGAS_UnitAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGAS_UnitAbility(){};

	//Visualization
	UFUNCTION(BlueprintCallable, Category = "Action Base")
		virtual void UpdateVisualization(AGridTile* PotentialTarget) {};
	UFUNCTION(BlueprintCallable, Category = "Action Base")
		virtual void ShowHoveredVisualization(AGridTile* PotentialTarget) {};
	UFUNCTION(BlueprintCallable, Category = "Action Base")
		virtual void HideHoveredVisualization() {};


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action Base")
		void SupplyTileToAbility(class AGridTile* Target);




public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		FText AbilityLabel;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		TSubclassOf<UCategoryData> AbilityCategory;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		bool bCanOnlyBeExecutedByOwner = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability Settings")
		bool bCanOnlyBeExecutedOnTurn = true;

	//UPROPERTY(BlueprintAssignable)
	//	FAbilityDelegate OnAbilityEnded;
};