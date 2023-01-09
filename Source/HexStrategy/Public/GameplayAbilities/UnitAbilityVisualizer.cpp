#include "UnitAbilityVisualizer.h"

void UUnitAbilityVisualizer::Tick(float DeltaTime)
{
	TickVisualizer(DeltaTime);
}

TStatId UUnitAbilityVisualizer::GetStatId() const
{
	return GetStatID();
}

bool UUnitAbilityVisualizer::IsTickable() const
{
	return this != this->GetClass()->GetDefaultObject();
}

bool UUnitAbilityVisualizer::IsTickableInEditor() const
{
	return false;
}

bool UUnitAbilityVisualizer::IsAllowedToTick() const
{
	return !IsTemplate(); ;
}

void UUnitAbilityVisualizer::UpdateVisualization_Implementation(AGridTile* PotentialTarget)
{

}

void UUnitAbilityVisualizer::ShowHoveredVisualization_Implementation(AGridTile* PotentialTarget)
{

}

void UUnitAbilityVisualizer::HideHoveredVisualization_Implementation()
{

}

void UUnitAbilityVisualizer::TickVisualizer_Implementation(float DeltaTime)
{

}

