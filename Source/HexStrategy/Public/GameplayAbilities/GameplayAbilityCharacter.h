// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS_AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "UnitAttributeSet.h"
#include "PaperCharacter.h"
#include "Units/Affiliation.h"
#include "GameplayAbilityCharacter.generated.h"

UCLASS(BlueprintType, Blueprintable)
class HEXSTRATEGY_API AUnitBase : public APaperCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnitBase();

	//BEGIN Gameplay Ability System functionality
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GAS", meta = (AllowPrivateAccess = "true"))
		class UGAS_AbilitySystemComponent* AbilitySystemComponent;
	/*
	* The gameplay attributes of the unit
	*/
	UPROPERTY()
		class UGAS_UnitAttributeSet* Attributes;
	/*
	* The Gameplay Effect class used to initialise this units attributes
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS");
		class TSubclassOf<class UGameplayEffect> DefaultAttributeSetter;
	/*
	* The list of abilities this character has by default
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS");
	TArray<TSubclassOf<class UGAS_UnitAbility>> DefaultAbilities;
	//END Gameplay Ability System functionality




	//BEGIN Unit Actor functionality
	/*
	* The Data Asset describing all the units appearance, behaviour, etc.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Attributes")
		class UUnitData* Unit;
	/*
	* The struct managing who controls this unit, should be set by whoever spawns it
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit|Attributes")
			FUnitAffiliation Affiliation {
			EUnitAffiliation::Neutral0
	};
	//END Unit functionality



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent)
		void InitializeAttributes();
	virtual void GiveAbilities();

	virtual void OnRep_PlayerState() override;
	void PossessedBy(AController* NewController) override;



private:
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
