// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	const int32 MaxNumSaveGames = 100;

public:
	//UFUNCTION(BlueprintCallable)
		
};
