#include "Grid/GridTile.h"
#include "Grid/GridActor.h"
#include "Grid/GridPainter/GridPainter.h"
#include "GameplayTasks/Classes/GameplayTask.h"
#include "Kismet/KismetMathLibrary.h"

AGridTile::AGridTile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
}

void AGridTile::SetTileState(const UObject* Agent, ETileDisplayState NewState, int32 InLayer /*= 0*/)
{
	DisplayState.SetTileState(Agent, NewState, InLayer);
	UpdateTile();
}

void AGridTile::ResetTileState(const UObject* Agent)
{
	DisplayState.ResetTileState(Agent);
	UpdateTile();
}

void AGridTile::UpdateTile()
{
	ParentGrid->GetGridPainter()->UpdateTile(this);
}

bool AGridTile::OccupyTile(AActor* InOccupyingActor)
{
	if (!OccupyingActor)
	{
		OccupyingActor = InOccupyingActor;
		return true;
	}

	return false;
}



void AGridTile::TryLeaveTile(AActor* InOccupyingActor, std::function<void(bool)> OnComplete)

{
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [=]()
		{
			UE_LOG(LogTemp, Log, TEXT("OnLeave Tasks bound to tile: %d"), OnLeaveTasks.Num());

			bool bMovementUnimpeded = true;

			for (int i = 0; i < OnLeaveTasks.Num() && bMovementUnimpeded; i++)
			{
				UE_LOG(LogTemp, Log, TEXT("Executing Async Task #%d"), i)

				UGameplayTask* TaskToExecute = OnLeaveTasks[i];

				TaskToExecute->ReadyForActivation();

				//Await completion of the task
				while (!TaskToExecute->IsFinished())
				{
					FPlatformProcess::Sleep(0.0f);
				}

				bMovementUnimpeded = ILeaveTileTask::Execute_GetMovementUnimpeded(TaskToExecute);

				UE_LOG(LogTemp, Log, TEXT("Executing Async Task #%d"), i)
			}


				// Once the task is done schedule a task back to the gamethread.
				// This is optional depending on the situation, you don't have to go back to the mainthread. 

				AsyncTask(ENamedThreads::GameThread, [=]()
					{
						if (OccupyingActor && OccupyingActor == InOccupyingActor)
						{
							OccupyingActor = nullptr;
							OnComplete(true);
							return;
						}

						OnComplete(false);
					});
		});
}

void AGridTile::AbandonTile(AActor* InOccupyingActor)
{
	if (OccupyingActor && OccupyingActor == InOccupyingActor)
	{
		OccupyingActor = nullptr;
	}
}

AActor* AGridTile::GetOccupyingUnit()
{
	return OccupyingActor;
}

bool AGridTile::GetIsOccupied()
{
	return (OccupyingActor != nullptr);
}

void FTileStateLayer::SetTileStateForLayer(const UObject* Agent, ETileDisplayState NewState)
{
	auto* ExistingEntry = LayerContent.FindByPredicate([=](TPair<const UObject*, ETileDisplayState> entry)
		{
			return entry.Key == Agent;
		});

	if (ExistingEntry)
	{
		if (NewState != ETileDisplayState::Default)
		{
			//Overwrite the existing state
			ExistingEntry->Value = NewState;
		}
		else
		{
			//Remove the existing entry, since ShowAsDefault should not be an overwrite state;
			ResetTileStateForLayer(Agent);
		}
	}
	else
	{
		if (NewState != ETileDisplayState::Default)
		{
			//Create new entry for this layer
			LayerContent.Add(TPair<const UObject*, ETileDisplayState>(Agent, NewState));
		}
	}
}

void FTileStateLayer::ResetTileStateForLayer(const UObject* Agent)
{
	auto* ExistingEntry = LayerContent.FindByPredicate([=](TPair<const UObject*, ETileDisplayState> entry)
		{
			return entry.Key == Agent;
		});

	if (ExistingEntry)
	{
		auto temp = *ExistingEntry;
		LayerContent.Remove(temp);
	}
}

ETileDisplayState FTileStateLayer::GetTileStateForLayer()
{
	if (LayerContent.Num() == 0)
	{
		return ETileDisplayState::Default;
	}
	else
	{
		return LayerContent[LayerContent.Num() - 1].Value;
	}
}

void FTileState::SetTileState(const UObject* Agent, ETileDisplayState NewState, int32 LayerIndex)
{
	ResetTileState(Agent);

	auto* LayerRef = Layers.Find(LayerIndex);
	if (LayerRef)
	{
		LayerRef->SetTileStateForLayer(Agent, NewState);
	}
	else
	{
		Layers.Add(LayerIndex, FTileStateLayer());
		LayerRef = Layers.Find(LayerIndex);
		LayerRef->SetTileStateForLayer(Agent, NewState);
	}
}

void FTileState::ResetTileState(const UObject* Agent)
{
	for (auto&& KVPair : Layers)
	{
		KVPair.Value.ResetTileStateForLayer(Agent);
	}

	int i = 0;
	TArray<int32> LayerKeys;
	Layers.GetKeys(LayerKeys);
	while (i < LayerKeys.Num())
	{
		auto* Layer = Layers.Find(LayerKeys[i]);
		if (Layer->LayerContent.Num() == 0)
		{
			Layers.Remove(LayerKeys[i]);
		}

		i++;
	}
}

ETileDisplayState FTileState::GetTileState()
{
	if (Layers.Num() == 0)
		return ETileDisplayState::Default;

	TArray<int32> LayerKeys;
	Layers.GetKeys(LayerKeys);
	int32 MaxLayer, index;
	UKismetMathLibrary::MaxOfIntArray(LayerKeys, index, MaxLayer);

	return Layers.Find(MaxLayer)->GetTileStateForLayer();

}
