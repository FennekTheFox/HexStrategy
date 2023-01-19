#include "Races.h"

UBodyTypeAppearanceData* UUnitRace::FindAppearanceData(FName SuperBodyType, FName SubBodyType)

{
	for (FSuperBodyTypeOptions& Opt : BodyTypeOptions)
	{
		if (Opt.OptionID == SuperBodyType)
		{
			for (FSubBodyTypeOptions& SubOpt : Opt.Suboptions)
			{
				if (SubOpt.OptionID == SubBodyType)
				{
					return SubOpt.Appearance;
				}
			}
		}
	}

	return nullptr;
}

FName UUnitRace::GetSuperBodyTypeIDForAppearance(UBodyTypeAppearanceData* Appearance)
{
	for (auto& SuperOpt : BodyTypeOptions)
	{
		for (auto& SubOpt : SuperOpt.Suboptions)
		{
			if (SubOpt.Appearance == Appearance)
			{
				return SuperOpt.OptionID;
			}
		}
	}
	return NAME_None;
}

FName UUnitRace::GetSubBodyTypeIDForAppearance(UBodyTypeAppearanceData* Appearance)
{
	for (auto& SuperOpt : BodyTypeOptions)
	{
		for (auto& SubOpt : SuperOpt.Suboptions)
		{
			if (SubOpt.Appearance == Appearance)
			{
				return SubOpt.OptionID;
			}
		}
	}
	return NAME_None;
}
