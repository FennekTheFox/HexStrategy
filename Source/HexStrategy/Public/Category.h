#pragma once

#include "Engine/DataAsset.h"

#include "Category.generated.h"


//General category data asset. Defines categories to have
//A localizable lable, tag and priority
UCLASS(BlueprintType)
class UCategoryData : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CategorySettings")
		FText CategoryLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CategorySettings")
		FName CategoryName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CategorySettings")
		int32 CategoryPriority;
};
