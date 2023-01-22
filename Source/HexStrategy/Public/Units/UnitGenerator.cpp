#include "UnitGenerator.h"

#include "UnitData.h"
#include "UnitOptionHelpers.h"
#include "Professions.h"

class UUnitData* UUnitGenerator::GenerateNewUnit(FUnitGenerationParameters& Params, UObject* Outer)
{
	UUnitData* OutUnitData = NewObject<UUnitData>();

	FUnitBodyData OutBodyData;

	//Pick a random race template for the unit
	auto** race_ptr = PickRandomWeightedValue<UUnitRace*, float>(Params.RaceWeights);
	UUnitRace* ChosenRace = (race_ptr ? *race_ptr : nullptr);

	if(!ensureMsgf(ChosenRace, TEXT("Error: Unit generation was unable to determine an appropriate race template."))) return nullptr;

	//Tally the weights of all the body types
	TMap<UBodyTypeAppearanceData*, float> PossibleBodyTypes;

	for (const FSuperBodyTypeOptions& SuperOpt : ChosenRace->BodyTypeOptions)
	{
		//Make sure the appearance has all required and no forbidden tags
		if (SuperOpt.SuperBodyTypeTags.HasAllExact(Params.RequiredBodyTypeTags) && !SuperOpt.SuperBodyTypeTags.HasAnyExact(Params.ForbiddenBodyTypeTags))
		{
			float SuperBodyTypeWeight = 0.f;

			//Tally up the weight for the super body type weight
			for (const auto& KVPair : Params.BodyTypeTagWeights)
			{
				SuperBodyTypeWeight += KVPair.Value * (float)SuperOpt.SuperBodyTypeTags.HasTagExact(KVPair.Key);
			}

			for (const FSubBodyTypeOptions& SubOpt : SuperOpt.Suboptions)
			{
				float AppearanceWeight = 0.f;

				UBodyTypeAppearanceData* AppearanceData = SubOpt.Appearance;
				if (!ensure(AppearanceData)) return nullptr;

				//Make sure the appearance has all required and no forbidden tags
				if (AppearanceData->BodyTypeTags.HasAllExact(Params.RequiredBodyTypeTags) && !AppearanceData->BodyTypeTags.HasAnyExact(Params.ForbiddenBodyTypeTags))
				{
					//Tally up the weight for the sub body type weight
					for (const auto& KVPair : Params.BodyTypeTagWeights)
					{
						AppearanceWeight += KVPair.Value * (float)AppearanceData->BodyTypeTags.HasTagExact(KVPair.Key);
					}

					PossibleBodyTypes.Add(AppearanceData, SuperBodyTypeWeight + AppearanceWeight);
				}
			}
		}
	}

	auto** appearance_ptr = PickRandomWeightedValue<UBodyTypeAppearanceData*, float>(PossibleBodyTypes);
	UBodyTypeAppearanceData* ChosenAppearance = (appearance_ptr ? *appearance_ptr : nullptr);

	if(!ensureMsgf(ChosenAppearance, TEXT("Error: Unit generation was unable to determine an appropriate appearance template."))) return nullptr;


	//Configure the output unit according to the rule and racial parameters
	FUnitBodyData BodyData;
	BodyData.UnitRace = FSoftObjectPath(ChosenRace);
	BodyData.SuperBodyTypeID = ChosenRace->GetSuperBodyTypeIDForAppearance(ChosenAppearance);
	BodyData.SubBodyTypeID = ChosenRace->GetSubBodyTypeIDForAppearance(ChosenAppearance);
	BodyData.BodyColor = OptionHelpers::PickRandomOption<FColourOption>(ChosenAppearance->PossibleBodyColors)->Colour;
	BodyData.EyeColor = OptionHelpers::PickRandomOption<FColourOption>(ChosenAppearance->PossibleEyeColors)->Colour;
	BodyData.HairColor = OptionHelpers::PickRandomOption<FColourOption>(ChosenAppearance->PossibleHairColors)->Colour;
	BodyData.HairStyle = OptionHelpers::PickRandomOption<FHairStypeOption>(ChosenAppearance->PossibleHairStyles)->HairStyle;

	OutUnitData->UnitBodyData = BodyData;


	//Pick a random profession for the unit
	auto** profession_ptr = PickRandomWeightedValue<UUnitProfession*, float>(Params.ProfessionWeights);
	OutUnitData->UnitProfession = (profession_ptr ? *profession_ptr : nullptr);


	return OutUnitData;
}