#include "TriggerZone.h"

#include <Components/BoxComponent.h>

void UTriggerEvent::Execute_Implementation(AActor* TriggeringActor)
{
	ensureMsgf(false, TEXT("Trigger event base implementation called. Any trigger event class should override this function."));
}

UWorld* UTriggerEvent::GetWorld() const
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

ATriggerZone::ATriggerZone()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerZone::OnTriggerOverlap);
}

void ATriggerZone::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Only execute this code on the side that has authority over the object
	if(HasAuthority())
		TriggeringEvent->Execute(OtherActor);
}
