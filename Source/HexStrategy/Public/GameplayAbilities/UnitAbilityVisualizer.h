#pragma once

#include "UnitAbilityVisualizer.generated.h"

//Base class for providing the ability visualization functionality
UCLASS(BlueprintType, Blueprintable)
class UUnitAbilityVisualizer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(ExposeOnSpawn))
		AActor* Owner;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (ExposeOnSpawn))
		TSubclassOf<class UGAS_UnitAbility> AbilityClass;

public:
	//Visualization
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action Base")
		void UpdateVisualization(AGridTile* PotentialTarget);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action Base")
		void ShowHoveredVisualization(AGridTile* PotentialTarget);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action Base")
		void HideHoveredVisualization();
};