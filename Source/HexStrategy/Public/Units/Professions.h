#pragma once

#include "Races.h"


#include "Professions.generated.h"



UCLASS(BlueprintType, Blueprintable)
class UUnitProfession : public UDataAsset
{
	GENERATED_BODY()

	UUnitProfession()
	{
		Refresh();
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText ProfessionLabel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<EBodyType, USpriteFlipbookSuit*> ProfessionFlipbooks;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bEquipmentHidesHair = false;



protected:
	//Makes sure all keys and subkeys are always present
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override
	{
		Refresh();
	}

private:
	void Refresh()
	{
		for (EBodyType BT : TEnumRange<EBodyType>())
		{
			ProfessionFlipbooks.FindOrAdd(BT);
		}
	}

	//... stuff like skin requirements here

};
