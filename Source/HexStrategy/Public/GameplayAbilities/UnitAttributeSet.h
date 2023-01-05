#pragma once


#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "UnitAttributeSet.generated.h"


//define user macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \


UCLASS()
class UGAS_UnitAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGAS_UnitAttributeSet();

	//Defines which properties are being replicated to the server and other players
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	//Unit Health
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, MaxHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);


	//Unit Current Health
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_CurrentHealth)
		FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, CurrentHealth);

	UFUNCTION()
		virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth);


	//Unit MP
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MP)
	FGameplayAttributeData MP;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, MP);

	UFUNCTION()
	virtual void OnRep_MP(const FGameplayAttributeData& OldMP);


	//Unit Physical Attack
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_PhysicalAttack)
		FGameplayAttributeData PhysicalAttack;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, PhysicalAttack);

	UFUNCTION()
		virtual void OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack);


	//Unit Magical Attack
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MagicAttack)
		FGameplayAttributeData MagicAttack;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, MagicAttack);

	UFUNCTION()
		virtual void OnRep_MagicAttack(const FGameplayAttributeData& OldMagicAttack);


	//PhysicalDefense
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_PhysicalDefense)
		FGameplayAttributeData PhysicalDefense;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, PhysicalDefense);

	UFUNCTION()
		virtual void OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense);


	//Template
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MagicDefense)
		FGameplayAttributeData MagicDefense;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, MagicDefense);

	UFUNCTION()
		virtual void OnRep_MagicDefense(const FGameplayAttributeData& OldMagicDefense);







	//Action Points (Turn Resources)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_ActionPoints)
		FGameplayAttributeData ActionPoints;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, ActionPoints);

	UFUNCTION()
		virtual void OnRep_ActionPoints(const FGameplayAttributeData& OldActionPoints);


	//BonusAction Points (Turn Resources)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_BonusActionPoints)
		FGameplayAttributeData BonusActionPoints;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, BonusActionPoints);

	UFUNCTION()
		virtual void OnRep_BonusActionPoints(const FGameplayAttributeData& OldBonusActionPoints);


	//MaxBonusAction Points (Turn Resources)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxBonusActionPoints)
		FGameplayAttributeData MaxBonusActionPoints;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, MaxBonusActionPoints);

	UFUNCTION()
		virtual void OnRep_MaxBonusActionPoints(const FGameplayAttributeData& OldBonusActionPoints);


	////Template
	//UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_)
	//	FGameplayAttributeData ;
	//ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, );

	//UFUNCTION()
	//	virtual void OnRep_(const FGameplayAttributeData& Old);



	//Unit Movement resource
	UPROPERTY(BlueprintReadOnly, CategorY = "Attributes", ReplicatedUsing = OnRep_TurnMovement)
		FGameplayAttributeData TurnMovement;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, TurnMovement);

	UFUNCTION()
		virtual void OnRep_TurnMovement(const FGameplayAttributeData& OldMovement);




	//Unit Movement resource
	UPROPERTY(BlueprintReadOnly, CategorY = "Attributes", ReplicatedUsing = OnRep_AvailableMovement)
		FGameplayAttributeData AvailableMovement;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, AvailableMovement);

	UFUNCTION()
	virtual void OnRep_AvailableMovement(const FGameplayAttributeData& OldMovement);



	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

};
