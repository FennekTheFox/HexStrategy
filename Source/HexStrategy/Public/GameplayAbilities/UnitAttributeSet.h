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
	UPROPERTY(BlueprintReadOnly, CategorY="Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, Health);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);


	//Unit MP
	UPROPERTY(BlueprintReadOnly, CategorY = "Attributes", ReplicatedUsing = OnRep_MP)
	FGameplayAttributeData MP;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, MP);

	UFUNCTION()
	virtual void OnRep_MP(const FGameplayAttributeData& OldMP);


	//Unit Movement resource
	UPROPERTY(BlueprintReadOnly, CategorY = "Attributes", ReplicatedUsing = OnRep_Movement)
		FGameplayAttributeData Movement;
	ATTRIBUTE_ACCESSORS(UGAS_UnitAttributeSet, Movement);

	UFUNCTION()
	virtual void OnRep_Movement(const FGameplayAttributeData& OldMovement);
};
