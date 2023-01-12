#pragma once

#include"TriggerEvent.generated.h"


UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew)
class UTriggerEvent : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "TriggerEvent")
		void Execute(AActor* TriggeringActor);

	UWorld* GetWorld() const override;
};
