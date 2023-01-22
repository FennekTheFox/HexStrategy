#pragma once

namespace OptionHelpers
{
	template <typename OptionType>
	static const OptionType* PickRandomOption(const TArray<OptionType>& Options)
	{
		if (!ensure(!Options.Num() == 0)) return nullptr;

		float CombinedWeight = 0;

		//Tally up the combined weights
		for (const OptionType& Option : Options)
		{
			CombinedWeight += Option.Weight;
		}

		//Pick a random value between 0  and the combined weight
		float RandomScalar = FMath::FRand();
		float TargetValue = CombinedWeight * RandomScalar;

		//Find the first entry in the distribution map where the running count exceeds the target value
		float RunningCount = 0;
		for (const OptionType& Option : Options)
		{
			RunningCount += Option.Weight;
			if (RunningCount >= TargetValue)
				return &Option;
		}

		return nullptr;
	}
}