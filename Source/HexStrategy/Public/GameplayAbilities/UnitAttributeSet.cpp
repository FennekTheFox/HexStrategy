#include "UnitAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"


UGAS_UnitAttributeSet::UGAS_UnitAttributeSet()
{

}

void UGAS_UnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, MP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, AvailableMovement, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, PhysicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, MagicAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, PhysicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, MagicDefense, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, ActionPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, BonusActionPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, MaxBonusActionPoints, COND_None, REPNOTIFY_Always);
}

void UGAS_UnitAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, MaxHealth, OldHealth);
}

void UGAS_UnitAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, CurrentHealth, OldHealth);
}

void UGAS_UnitAttributeSet::OnRep_MP(const FGameplayAttributeData& OldMP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, MP, OldMP);
}

void UGAS_UnitAttributeSet::OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, PhysicalAttack, OldPhysicalAttack);
}

void UGAS_UnitAttributeSet::OnRep_MagicAttack(const FGameplayAttributeData& OldMagicAttack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, MagicAttack, OldMagicAttack);
}

void UGAS_UnitAttributeSet::OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, PhysicalDefense, OldPhysicalDefense);
}

void UGAS_UnitAttributeSet::OnRep_MagicDefense(const FGameplayAttributeData& OldMagicDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, MagicDefense, OldMagicDefense);
}

void UGAS_UnitAttributeSet::OnRep_ActionPoints(const FGameplayAttributeData& OldActionPoints)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, ActionPoints, OldActionPoints);
}

void UGAS_UnitAttributeSet::OnRep_BonusActionPoints(const FGameplayAttributeData& OldBonusActionPoints)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, BonusActionPoints, OldBonusActionPoints);
}

void UGAS_UnitAttributeSet::OnRep_MaxBonusActionPoints(const FGameplayAttributeData& OldBonusActionPoints)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, MaxBonusActionPoints, OldBonusActionPoints);
}

void UGAS_UnitAttributeSet::OnRep_TurnMovement(const FGameplayAttributeData& OldMovement)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, TurnMovement, OldMovement);
}

void UGAS_UnitAttributeSet::OnRep_AvailableMovement(const FGameplayAttributeData& OldMovement)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, AvailableMovement, OldMovement);
}

void UGAS_UnitAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute() || Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		//If the edited effect was our health clamp it between 0 and max
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetActionPointsAttribute())
	{
		//If the edited effect was our health clamp it between 0 and max
		SetActionPoints(FMath::Clamp(GetActionPoints(), 0.f, 1.f));
	}

	if (Data.EvaluatedData.Attribute == GetBonusActionPointsAttribute() || Data.EvaluatedData.Attribute == GetMaxBonusActionPointsAttribute())
	{
		//If the edited effect was our health clamp it between 0 and max
		SetBonusActionPoints(FMath::Clamp(GetBonusActionPoints(), 0.f, GetMaxBonusActionPoints()));
	}
}
