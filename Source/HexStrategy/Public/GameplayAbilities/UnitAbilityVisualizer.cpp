#include "UnitAbilityVisualizer.h"

void UUnitAbilityVisualizer::Tick(float DeltaTime)
{
	TickVisualizer(DeltaTime);
}

TStatId UUnitAbilityVisualizer::GetStatId() const
{
	return GetStatID();
}


ETickableTickType UUnitAbilityVisualizer::GetTickableTickType() const
{
	return IsTemplate() ? ETickableTickType::Never : FTickableGameObject::GetTickableTickType();
}

bool UUnitAbilityVisualizer::IsTickableInEditor() const
{
	return false;
}

bool UUnitAbilityVisualizer::IsAllowedToTick() const
{
	return !IsTemplate() &&  !IsUnreachable();
}

void UUnitAbilityVisualizer::UpdateVisualization_Implementation(UGridTile* PotentialTarget)
{

}

void UUnitAbilityVisualizer::ShowHoveredVisualization_Implementation(UGridTile* PotentialTarget)
{

}

void UUnitAbilityVisualizer::HideHoveredVisualization_Implementation()
{

}

void UUnitAbilityVisualizer::TickVisualizer_Implementation(float DeltaTime)
{

}

