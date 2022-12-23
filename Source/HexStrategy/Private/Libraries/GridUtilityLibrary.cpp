// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/GridUtilityLibrary.h"
#include "Grid/GridActor.h"
#include "GameplayAbilitySpecHandle.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"

int UGridUtilityLibrary::GetHexDistance_FromCoords(FIntVector A, FIntVector B)
{
	FIntVector D = B - A;
	if (FMath::Sign(D.X) == FMath::Sign(D.Y))
	{
		return FMath::Abs(D.X + D.Y);
	}
	else
	{
		return FMath::Max(FMath::Abs(D.X), FMath::Abs(D.Y));
	}
}

int UGridUtilityLibrary::GetHexDistance_FromTiles(AGridTile* A, AGridTile* B)
{
	return GetHexDistance_FromCoords(A->Coordinates, B->Coordinates);
}

bool UGridUtilityLibrary::CommitCheck(UGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	return Ability->CommitCheck(Handle, &ActorInfo, ActivationInfo);
}

