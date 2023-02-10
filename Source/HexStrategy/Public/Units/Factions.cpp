#include "Factions.h"
#include <Net/UnrealNetwork.h>

void UUnitFactionData::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUnitFactionData, Affiliation_DisplayName);
	DOREPLIFETIME(UUnitFactionData, Affiliation_ID);
	DOREPLIFETIME(UUnitFactionData, bUseAffiliationColorOverride);
	DOREPLIFETIME(UUnitFactionData, ColorOverride);
}

