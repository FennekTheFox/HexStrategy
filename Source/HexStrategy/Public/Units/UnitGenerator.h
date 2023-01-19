#pragma once

#include <GameplayTagContainer.h>

#include "Races.h"

#include "UnitGenerator.generated.h"


USTRUCT(BlueprintType)
struct FUnitGenerationParameters
{
	GENERATED_BODY()

public:
	/*The possible races for the generated unit with their respective weights*/
	UPROPERTY(BlueprintReadWrite, Category = "Unit Generation Parameters|Appearance")
		TMap<class UUnitRace*, float> RaceWeights;
	/*The tags that the generated unit has to have*/
	UPROPERTY(BlueprintReadWrite, Category = "Unit Generation Parameters|Appearance")
		FGameplayTagContainer RequiredBodyTypeTags;
	/*The tags that the generated unit is not allowed to have*/
	UPROPERTY(BlueprintReadWrite, Category = "Unit Generation Parameters|Appearance")
		FGameplayTagContainer ForbiddenBodyTypeTags;
	/*Maps how important each */
	UPROPERTY(BlueprintReadWrite, Category = "Unit Generation Parameters|Appearance")
		TMap<FGameplayTag, float> BodyTypeTagWeights;


	//The weighting of possible professons 
	UPROPERTY(BlueprintReadWrite, Category = "Unit Generation Parameters|Appearance")
		TMap<class UUnitProfession*, float> ProfessionWeights;

};

UCLASS(Abstract)
class UUnitGenerator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Units")
		static class UUnitData* GenerateNewUnit(UPARAM(ref)FUnitGenerationParameters& Params, UObject* Outer);


	template<typename T>
	void Foo(T s)
	{
		std::cout << s << '\n';
	}

	static FLinearColor PickRandomLinearColor(const TArray<FColourOption>& Options);
	static UHairStyle* PickRandomHairStyle(const TArray<FHairStypeOption>& Options);

	template<typename ValueType, typename WeightType>
	static ValueType* PickRandomWeightedValue(TMap<ValueType, WeightType>& DistributionMap)
	{
		WeightType CombinedWeight = 0;

		//Tally up the combined weights
		for (auto& KVPair : DistributionMap)
		{
			CombinedWeight += KVPair.Value;
		}

		//Pick a random value between 0  and the combined weight
		float RandomScalar = FMath::FRand();
		WeightType TargetValue = CombinedWeight * RandomScalar;

		//Find the first entry in the distribution map where the running count exceeds the target value
		WeightType RunningCount = 0;
		for (auto& KVPair : DistributionMap)
		{
			RunningCount += KVPair.Value;
			if (RunningCount >= TargetValue)
				return &KVPair.Key;
		}
		return nullptr;
	}
};