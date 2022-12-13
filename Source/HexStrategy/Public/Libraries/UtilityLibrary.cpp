#include "UtilityLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"

void UUtilityLibrary::FlushInput(UObject* WorldContextObject)
{
	UGameplayStatics::GetPlayerController(WorldContextObject, 0)->PlayerInput->FlushPressedKeys();
}

void UUtilityLibrary::SortObjectArray(TArray<UObject*>& OutArray, FObjectSortDelegate Comparator)
{
	TArray<UObject*> temp = OutArray;

	Algo::Sort(temp, [=](UObject* L, UObject* R)
		{
			bool ret = false;
			Comparator.ExecuteIfBound(L, R, ret);
			return ret;
		});

	OutArray = temp;
}

