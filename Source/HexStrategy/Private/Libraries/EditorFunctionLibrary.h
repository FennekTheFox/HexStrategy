// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EditorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

	UFUNCTION(BlueprintCallable, Category = "Editor Utility")
		static void SetSpriteAdditionalTexture(UPaperSprite* Sprite, UTexture* AdditionalTexture, int32 Index = 0);
};
