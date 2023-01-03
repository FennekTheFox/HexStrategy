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