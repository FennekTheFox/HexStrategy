// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Calc_PhysicalDamage.generated.h"

/**
 * 
 */


UCLASS()
class HEXSTRATEGY_API UCalc_PhysicalDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UCalc_PhysicalDamage();	

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
