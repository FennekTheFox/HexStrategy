#include "Factions.h"
#include <Net/UnrealNetwork.h>

void UUnitFactionData::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUnitFactionData, Faction_DisplayName);
	DOREPLIFETIME(UUnitFactionData, Faction_ID);
	DOREPLIFETIME(UUnitFactionData, ColorOverride);
	DOREPLIFETIME(UUnitFactionData, Relations);
}

EFactionRelationState UUnitFactionData::GetRelationTo(const UUnitFactionData* OtherFaction)
{
	if (FFactionRelationship* Relation = Relations.FindByPredicate([OtherFaction](const FFactionRelationship& r)
		{
			if (r.TargetFaction == OtherFaction)
				return true;

			return false;
		}))
	{
		return Relation->RelationState;
	}

	return EFactionRelationState::Neutral;
}

