#include "HexStrategyGameState.h"

#include "SaveGame/GameFileData.h"
#include <Net/UnrealNetwork.h>
#include <Engine/ActorChannel.h>
#include <Net/DataBunch.h>


void UMyObject::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(UMyObject, Value);
}


//void AHexStrategyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	DOREPLIFETIME(AHexStrategyGameState, LoadedGameFileData);
//}
//
//bool AHexStrategyGameState::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
//{
//	//bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
//
//	//bWroteSomething |= Channel->ReplicateSubobjectList(LoadedGameFileData, *Bunch, *RepFlags);
//
//	//return bWroteSomething;
//}
//
