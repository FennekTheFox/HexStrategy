// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS_AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "UnitAttributeSet.h"
#include "PaperCharacter.h"
#include "GameplayAbilityCharacter.generated.h"

UCLASS(BlueprintType, Blueprintable)
class HEXSTRATEGY_API AGameplayAbilityCharacter : public APaperCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameplayAbilityCharacter();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GAS", meta = (AllowPrivateAccess = "true"))
		class UGAS_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class UGAS_UnitAttributeSet* Attributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS");
		class TSubclassOf<class UGameplayEffect> DefaultAttributeSetter;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS");
		TArray<TSubclassOf<class UGAS_UnitAbility>> DefaultAbilities;



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
