#pragma once

#include "TriggerEvent.h"

#include "TriggerZone.generated.h"


/*Class that provides a trigger box and a customizable event that it triggers upon overlapping*/
UCLASS(BlueprintType, hideCategories = ("Rendering", "HLOD", "Physics", "Input", "Cooking"))
class ATriggerZone : public AActor
{
	GENERATED_BODY()

	ATriggerZone();

public: 
	UPROPERTY(BlueprintReadOnly, Instanced, EditAnywhere)
		UTriggerEvent* TriggeringEvent;

private:
	UPROPERTY()
		class UBoxComponent* TriggerBox;

	UFUNCTION()
		void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};