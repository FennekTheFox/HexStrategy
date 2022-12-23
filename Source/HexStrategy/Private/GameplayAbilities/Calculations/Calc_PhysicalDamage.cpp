// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Calculations/Calc_PhysicalDamage.h"
#include "GameplayAbilities/UnitAttributeSet.h"


struct FPhysicalDamageFactors
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalAttack)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth)

	FPhysicalDamageFactors()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAS_UnitAttributeSet, PhysicalAttack, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAS_UnitAttributeSet, PhysicalDefense, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGAS_UnitAttributeSet, CurrentHealth, Target, true);
	}
};

static const FPhysicalDamageFactors& PhysicalDamageFactors()
{
	static FPhysicalDamageFactors f;
	return f;
}

UCalc_PhysicalDamage::UCalc_PhysicalDamage()
{
	RelevantAttributesToCapture.Add(PhysicalDamageFactors().PhysicalAttackDef);
	RelevantAttributesToCapture.Add(PhysicalDamageFactors().PhysicalDefenseDef);
	RelevantAttributesToCapture.Add(PhysicalDamageFactors().CurrentHealthDef);
}

void UCalc_PhysicalDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Get owner and source references
	UAbilitySystemComponent* TargetABSC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetABSC ? TargetABSC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;

	//Get owning effect spec
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//Capture the potency modifier
	float BaseDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attack.Potency")), false, -1.0f), 0.0f);

	float PhysicalAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysicalDamageFactors().PhysicalAttackDef, EvaluationParameters, PhysicalAttack);
	float PhysicalDefense = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysicalDamageFactors().PhysicalDefenseDef, EvaluationParameters, PhysicalDefense);
	float CurrentHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysicalDamageFactors().CurrentHealthDef, EvaluationParameters, CurrentHealth);

	//Actual damage calculation

	float Ratio = (PhysicalAttack / PhysicalDefense);
	float DamageFactor = (Ratio > 1.f ? 1 + FMath::Log2(Ratio) : Ratio * Ratio);
	float Damage = DamageFactor * BaseDamage;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(PhysicalDamageFactors().CurrentHealthProperty, EGameplayModOp::Additive, -Damage));
}
