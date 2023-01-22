// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridUtilityLibrary.generated.h"

/**
 *
 */
UCLASS()
class UGridUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Grid Utility", meta = (DisplayName = "Get Hex Distance from Coords", CompactNodeTitle = "Hex Distance"))
		static int GetHexDistance_FromCoords(FIntVector A, FIntVector B);
	UFUNCTION(BlueprintPure, Category = "Grid Utility", meta = (DisplayName = "Get Hex Distance from Tiles", CompactNodeTitle = "Hex Distance"))
		static int GetHexDistance_FromTiles(UGridTile* A, UGridTile* B);

	UFUNCTION(BlueprintCallable)
		static bool CommitCheck(UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

};
