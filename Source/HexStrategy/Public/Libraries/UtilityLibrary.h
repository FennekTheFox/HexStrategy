// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityLibrary.generated.h"


DECLARE_DYNAMIC_DELEGATE_ThreeParams(FObjectSortDelegate, UObject*, Obj1, UObject*, Obj2, bool&, Value);


/**
 *
 */
UCLASS()
class HEXSTRATEGY_API UUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = WorldContextObject))
		static void FlushInput(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable)
		static void SortObjectArray(UPARAM(ref)TArray<UObject*>& Array, FObjectSortDelegate Comparator);
};
