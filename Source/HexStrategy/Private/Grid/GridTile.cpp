#include "Grid/GridTile.h"
#include "GameplayTasks/Classes/GameplayTask.h"

AGridTile::AGridTile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
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

