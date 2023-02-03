// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_PlayFromMapping.h"
#include "AnimSequences/Players/PaperZDAnimPlayer.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "PaperZDAnimMapping.h"

FPaperZDAnimNode_PlayFromMapping::FPaperZDAnimNode_PlayFromMapping()
	: AnimMapping(nullptr)
	, PlayRate(1.0f)
	, StartPosition(0.0f)
	, bLoopAnimation(true)
	, PlaybackTime(0.0f)
{}

void FPaperZDAnimNode_PlayFromMapping::OnInitialize(const FPaperZDAnimationInitContext& InitContext)
{
	if (AnimMapping)
	{
		UPaperZDAnimSequence** SequencePtr = AnimMapping->Mapping.Find(Key);

		if (SequencePtr)
		{
			UPaperZDAnimSequence* AnimSequence = *SequencePtr;
			//Initialize the starting time
			const float SeqDuration = AnimSequence->GetTotalDuration();
			PlaybackTime = PlayRate < 0.0f ? SeqDuration : FMath::Min(StartPosition, SeqDuration);

		}
	}
}

void FPaperZDAnimNode_PlayFromMapping::OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext)
{
	if (AnimMapping)
	{
		UPaperZDAnimSequence** SequencePtr = AnimMapping->Mapping.Find(Key);

		if (SequencePtr)
		{
			UPaperZDAnimSequence *AnimSequence = *SequencePtr;
			//Independent of the weight we have, we should update the playback, to avoid losing sync
			UPaperZDAnimPlayer* Player = UpdateContext.AnimInstance->GetPlayer();
			Player->TickPlayback(AnimSequence, PlaybackTime, UpdateContext.DeltaTime * PlayRate, bLoopAnimation, UpdateContext.AnimInstance, UpdateContext.Weight);
		}
	}
}

void FPaperZDAnimNode_PlayFromMapping::OnEvaluate(FPaperZDAnimationPlaybackData& OutData)
{
	if (AnimMapping)
	{
		UPaperZDAnimSequence** SequencePtr = AnimMapping->Mapping.Find(Key);

		if (SequencePtr)
		{
			UPaperZDAnimSequence* AnimSequence = *SequencePtr;
			//Forcefully add the animation as the only present
			OutData.SetAnimation(AnimSequence, PlaybackTime);
		}
	}
}

UPaperZDAnimSequence* FPaperZDAnimNode_PlayFromMapping::GetAnimSequence() const
{
	if (AnimMapping)
	{
		UPaperZDAnimSequence** SequencePtr = AnimMapping->Mapping.Find(Key);
		if (SequencePtr) return *SequencePtr;
	}

	return nullptr;
}

