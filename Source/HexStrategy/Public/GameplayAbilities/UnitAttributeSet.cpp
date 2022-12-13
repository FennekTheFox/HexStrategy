#include "UnitAttributeSet.h"
#include "Net/UnrealNetwork.h"


UGAS_UnitAttributeSet::UGAS_UnitAttributeSet()
{

}

void UGAS_UnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, MP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_UnitAttributeSet, Movement, COND_None, REPNOTIFY_Always);
}

void UGAS_UnitAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, Health, OldHealth);
}

void UGAS_UnitAttributeSet::OnRep_MP(const FGameplayAttributeData& OldMP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, MP, OldMP);
}

void UGAS_UnitAttributeSet::OnRep_Movement(const FGameplayAttributeData& OldMovement)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_UnitAttributeSet, Movement, OldMovement);
}

