#pragma once

#include <Tickable.h>
#include "UnitAbilityVisualizer.generated.h"

//Base class for providing the ability visualization functionality
UCLASS(BlueprintType, Blueprintable)
class UUnitAbilityVisualizer
: public UObject
, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(ExposeOnSpawn))
		AActor* Owner;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (ExposeOnSpawn))
		TSubclassOf<class UGAS_UnitAbility> AbilityClass;

	//BEGIN FTickableGameObject
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override;
	bool IsTickableInEditor() const override;
	bool IsAllowedToTick() const override;

	//End FTickableGameObject

public:
	//Visualization
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action Base")
		void UpdateVisualization(UGridTile* PotentialTarget);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action Base")
		void ShowHoveredVisualization(UGridTile* PotentialTarget);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action Base")
		void HideHoveredVisualization();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action Base")
		void TickVisualizer(float DeltaTime);
};