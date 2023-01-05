// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GameplayAbilityCharacter.h"
#include "GameplayAbilities/GAS_UnitAbilities.h"
#include "Units/UnitData.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include <Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AUnitBase::AUnitBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UGAS_AbilitySystemComponent>("Ability System Component");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);


	//GameplayAttributes = CreateDefaultSubobject<UGAS_UnitAttributeSet>("Attributes");
	//AbilitySystemComponent->AddAttributeSetSubobject(GameplayAttributes.Get());

	bReplicates = true;
	NetUpdateFrequency = 100.0f;
}

void AUnitBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnitBase, Unit);
	DOREPLIFETIME(AUnitBase, Affiliation);

}

void AUnitBase::UnPossessed()
{
	Super::UnPossessed();
	SpawnDefaultController();
}

void AUnitBase::PostInitializeComponents()
{
	InitializeAbilitySystem();

	Super::PostInitializeComponents();
}


// Called when the game starts or when spawned
void AUnitBase::BeginPlay()
{
	Super::BeginPlay();

	//We need to duplicate the unit object if its not transient, since we otherwise
	//override values in the template data assets
	if (Unit && Unit->GetPackage() != GetTransientPackage())
	{
		Unit = DuplicateObject<UUnitData>(Unit, GetTransientPackage());
		Unit->Initialize();
	}

	if (HasAuthority())
	{
		for (TSubclassOf<UGameplayEffect> GE : DefaultOnTurnStartGEs)
		{
			UGameplayEffect* newGE = NewObject<UGameplayEffect>(this, GE);
			InternalGEs.Add(newGE);
			OnTurnStartGEs.Add(UAbilitySystemBlueprintLibrary::MakeSpecHandle(newGE, this, this, 1.0f));
		}

		for (TSubclassOf<UGameplayEffect> GE : DefaultOnTurnEndGEs)
		{
			UGameplayEffect* newGE = NewObject<UGameplayEffect>(this, GE);
			InternalGEs.Add(newGE);
			OnTurnEndGEs.Add(UAbilitySystemBlueprintLibrary::MakeSpecHandle(newGE, this, this, 1.0f));
		}

		for (TSubclassOf<UGameplayEffect> GE : DefaultOnEncounterStartGEs)
		{
			UGameplayEffect* newGE = NewObject<UGameplayEffect>(this, GE);
			InternalGEs.Add(newGE);
			OnEncounterStartGEs.Add(UAbilitySystemBlueprintLibrary::MakeSpecHandle(newGE, this, this, 1.0f));
		}

		for (TSubclassOf<UGameplayEffect> GE : DefaultOnEncounterEndGEs)
		{
			UGameplayEffect* newGE = NewObject<UGameplayEffect>(this, GE);
			InternalGEs.Add(newGE);
			OnEncounterEndGEs.Add(UAbilitySystemBlueprintLibrary::MakeSpecHandle(newGE, this, this, 1.0f));
		}
	}
	
}

void AUnitBase::InitializeAbilitySystem()
{
	check(IsValid(ASC));

	// Initialize ASC on this Actor
	ASC->InitAbilityActorInfo(this, this);

	InitializeAttributes();


}

// Called every frame
void AUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUnitBase::InitializeAttributes_Implementation()
{
	if (ASC && Unit)
	{
		ASC->AddSet<UGAS_UnitAttributeSet>();

		ASC->SetNumericAttributeBase(UGAS_UnitAttributeSet::GetMaxHealthAttribute(), Unit->Attributes.Vigor.GetEffectiveValue() * 10.f);
		ASC->SetNumericAttributeBase(UGAS_UnitAttributeSet::GetCurrentHealthAttribute(), Unit->Attributes.Vigor.GetEffectiveValue() * 10.f);
		ASC->SetNumericAttributeBase(UGAS_UnitAttributeSet::GetPhysicalAttackAttribute(), Unit->Attributes.Ferocity.GetEffectiveValue() * 10.f);
		ASC->SetNumericAttributeBase(UGAS_UnitAttributeSet::GetPhysicalDefenseAttribute(), Unit->Attributes.Endurance.GetEffectiveValue() * 10.f);

		ASC->SetNumericAttributeBase(UGAS_UnitAttributeSet::GetMaxBonusActionPointsAttribute(), 3.f);
		ASC->SetNumericAttributeBase(UGAS_UnitAttributeSet::GetTurnMovementAttribute(), 3 +  Unit->Attributes.Swiftness.GetEffectiveValue()/10.f);

		//GameplayAttributes->InitCurrentHealth(Unit->Attributes.Vigor.GetEffectiveValue() * 10.f);
		//GameplayAttributes->InitMaxHealth(Unit->Attributes.Vigor.GetEffectiveValue() * 10.f);
	}
}

void AUnitBase::NotifyTurnStarted()
{
	if (ASC && Unit)
	{
		for (FGameplayEffectSpecHandle GE : OnTurnStartGEs)
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(GE);
		}
	}
}

void AUnitBase::NotifyTurnEnded()
{
	if (ASC && Unit)
	{
		for (FGameplayEffectSpecHandle GE : OnTurnEndGEs)
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(GE);
		}
	}
}


void AUnitBase::NotifyEncounterStarted()
{
	if (ASC && Unit)
	{
		for (FGameplayEffectSpecHandle GE : OnEncounterStartGEs)
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(GE);
		}
	}
}


void AUnitBase::NotifyEncounterEnded()
{
	if (ASC && Unit)
	{
		for (FGameplayEffectSpecHandle GE : OnEncounterEndGEs)
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(GE);
		}
	}
}


void AUnitBase::SetUnit_Implementation(UUnitData* InUnit, FUnitAffiliation InAffiliation)
{
	Unit = InUnit;
	Affiliation = InAffiliation;
}


void AUnitBase::GiveAbilities()
{
	if (HasAuthority() && ASC && !bInitialized)
	{
		for (TSubclassOf<UGAS_UnitAbility>& AbilityClass : DefaultAbilities)
		{
			//FGameplayAbilitySpec(TSubclassOf<UGameplayAbility> InAbilityClass, int32 InLevel = 1, int32 InInputID = INDEX_NONE, UObject * InSourceObject = nullptr);

			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(AbilityClass, 1, static_cast<int32>(INDEX_NONE), this);
			ASC->GiveAbility(Spec);
		}
		bInitialized = true;
	}
}

void AUnitBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Client GAS init
	
	//Maybe: Bind abilities? should happen differently here

	InitializeAttributes();
}


void AUnitBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Server GAS init
	ASC->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();
}

UAbilitySystemComponent* AUnitBase::GetAbilitySystemComponent() const
{
	return ASC;
}
