#include "InteractionComponent.h"


void UInteraction::OnExecuteInteraction_Implementation(UInteractionComponent* Component)
{

}

class UWorld* UInteraction::GetWorld() const
{
	if (HasAnyFlags(RF_ArchetypeObject | RF_ClassDefaultObject))
	{
		return nullptr;
	}

	// Get from outer
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			return World;
		}
	}

	// Last resort
	return nullptr;
}

UInteractionComponent::UInteractionComponent()
{
	///Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/Components/Interaction/W_InteractionDisplay.W_InteractionDisplay'
	static ConstructorHelpers::FClassFinder<UUserWidget> InteractionWidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/Components/Interaction/W_InteractionDisplay.W_InteractionDisplay_C'"));
	if (ensure(InteractionWidgetFinder.Succeeded()))
	{
		SetWidgetClass(InteractionWidgetFinder.Class);
	}
	Space = EWidgetSpace::Screen;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	FVector Origin, Extent;
	GetOwner()->GetActorBounds(false, Origin, Extent);
	SetRelativeLocation(FVector(0.f, 0.f, Extent.Z * (-1)));
	SetVisibility(false, true);
}

void UInteractionComponent::TriggerInteraction()
{
	Interaction->OnExecuteInteraction(this);
}	

void UInteractionComponent::SetInteractionVisible(bool newVisible)
{
	SetVisibility(newVisible, true);
}
