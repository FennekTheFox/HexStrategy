// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GameplayAbilityCharacter.h"
#include "GameplayAbilities/GAS_UnitAbilities.h"

// Sets default values
AGameplayAbilityCharacter::AGameplayAbilityCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>("Ability System Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UGAS_UnitAttributeSet>("Attributes");
}

// Called when the game starts or when spawned
void AGameplayAbilityCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameplayAbilityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameplayAbilityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGameplayAbilityCharacter::InitializeAttributes_Implementation()
{
	if (AbilitySystemComponent && DefaultAttributeSetter)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeSetter, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle =AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AGameplayAbilityCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGAS_UnitAbility>& AbilityClass : DefaultAbilities)
		{
			//FGameplayAbilitySpec(TSubclassOf<UGameplayAbility> InAbilityClass, int32 InLevel = 1, int32 InInputID = INDEX_NONE, UObject * InSourceObject = nullptr);

			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(AbilityClass, 1, static_cast<int32>(INDEX_NONE), this);
			AbilitySystemComponent->GiveAbility(Spec);
		}
	}
}

void AGameplayAbilityCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Client GAS init
	
	//Maybe: Bind abilities? should happen differently here

	InitializeAttributes();
}


void AGameplayAbilityCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Server GAS init
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();
}

UAbilitySystemComponent* AGameplayAbilityCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

