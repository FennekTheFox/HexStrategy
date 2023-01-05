#include "Grid/Components/GridMovementComponent.h"

#include "Grid/PathFinders/GridPathFinder.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
//#include "Grid/GridActions/GridAction_Move.h"
//#include "Grid/Actors/GridUnit.h"
#include "Curves/CurveFloat.h"
#include "GameplayAbilities/GAS_UnitAbilities.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemGlobals.h"
#include "GameplayTagContainer.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/UnitAttributeSet.h"
#include "Net/UnrealNetwork.h"



UGridMovementComponent::UGridMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	//Find movement curve assets
	static ConstructorHelpers::FObjectFinder<UCurveFloat> WalkCurveFinder(TEXT("/Script/Engine.CurveFloat'/Game/Assets/Data/Curves/GMC/GMC_Walk.GMC_Walk'"));
	if (WalkCurveFinder.Succeeded())
	{
		WalkCurve = WalkCurveFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> JCH_Finder(TEXT("/Script/Engine.CurveFloat'/Game/Assets/Data/Curves/GMC/GMC_Jump_Horizontal.GMC_Jump_Horizontal'"));
	if (JCH_Finder.Succeeded())
	{
		JumpCurve_Horizontal = JCH_Finder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> JCV_Finder(TEXT("/Script/Engine.CurveFloat'/Game/Assets/Data/Curves/GMC/GMC_Jump_Vertical.GMC_Jump_Vertical'"));
	if (JCV_Finder.Succeeded())
	{
		JumpCurve_Vertical = JCV_Finder.Object;
	}

	static ConstructorHelpers::FClassFinder<UGAS_UnitAbility> MoveAbilityFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Abilities/GameplayAbilities/GridMove/GA_GridMove.GA_GridMove_C'"));
	if (ensure(MoveAbilityFinder.Succeeded()))
	{
		MoveAbilityClass = MoveAbilityFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DecrementMovementFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Abilities/GameplayAbilities/GridMove/GE_GridMoveCost.GE_GridMoveCost_C'"));
	if (ensure(DecrementMovementFinder.Succeeded()))
	{
		DecrementMovementEffectClass = DecrementMovementFinder.Class;
	}

}

void UGridMovementComponent::AttachToGrid(AGridActor* NewGrid)
{
	if (NewGrid)
	{
		//If a grid has been passed, attach to it. If NewGrid is a nullptr, look for a grid
		Grid = NewGrid;
	}
	else
	{
		Grid = Cast<AGridActor>(UGameplayStatics::GetActorOfClass(this, AGridActor::StaticClass()));
	}

	//Initialise the path finders reference to the grid
	//PathFinder->GridActor = Grid;

	//Initiate movement to the closest tile	
	CurrentTileLoc = GetOwner()->GetActorLocation() - Bounds.Z * FVector::UpVector;
	NextTile = Grid->GetTileClosestToCoordinates(GetOwner()->GetActorLocation());
	ensure(NextTile->PreoccupyTile(GetOwner()));
	NextTileLoc = NextTile->GetActorLocation();
	NextTile->OccupyTile(GetOwner());
	MovementDuration = (CurrentTileLoc - NextTileLoc).Size() / MovementSpeed;
	TimePassed = 0.0f;
	MovementState = Walking;
	//CL_AttachToGrid_Implementation(NewGrid);

	bInMotion = true;
}

//void UGridMovementComponent::CL_AttachToGrid_Implementation(AGridActor* NewGrid /*= nullptr*/)
//{
//	//PathFinder->GridActor = NewGrid;
//}

void UGridMovementComponent::DetachFromGrid(AGridActor* InGrid /*= nullptr*/)
{
	if (!InGrid)
	{
		InGrid = Cast<AGridActor>(UGameplayStatics::GetActorOfClass(this, AGridActor::StaticClass()));
	}

	if (Grid == InGrid)
	{
		CurrentTile->AbandonTile(GetOwner());
		CurrentTile = nullptr;
		Grid = nullptr;
	}
}

bool UGridMovementComponent::MoveToTile(AGridTile* TargetTile)
{
	ensure(PathFinder);
	ensure(TargetTile);


	if (CurrentTile == TargetTile)
	{
		//Why move, when we're already there

		OnMovementCompleted();
		return true;
	}

	TArray<AGridTile*>Path;

	FGridPathFinderRequest Request;
	Request.GridActor = Grid;
	Request.Sender = GetOwner();
	Request.Start = CurrentTile;
	Request.Goal = TargetTile;
	Request.MaxCost = GetAvailableMovement();

	if (PathFinder->FindPath(Request, Path))
	{
		bInMotion = true;
		TimePassed = 0.f;

		//Clean the current tile from the path
		Path.Remove(CurrentTile);

		if (Path.Num() != 0)
		{
			PathToTravel = Path;
			return true;
		}
	}
	return false;
}






bool UGridMovementComponent::GetPathTo(AGridTile* TargetTile, TArray<AGridTile*>& OutPath)
{
	ensure(PathFinder);
	ensure(TargetTile);


	FGridPathFinderRequest Request;
	Request.GridActor = Grid;
	Request.Sender = GetOwner();
	Request.Start = CurrentTile;
	Request.Goal = TargetTile;
	Request.MaxCost = GetAvailableMovement();

	return PathFinder->FindPath(Request, OutPath);
}

void UGridMovementComponent::AbortMovement()
{
	UE_LOG(LogTemp, Log, TEXT("Abandoning Movement"))
	bInMotion = false;
	PathToTravel.Reset();
}

void UGridMovementComponent::PauseMovement()
{
	bPaused = true;
}

void UGridMovementComponent::ResumeMovement()
{
	bPaused = false;
}

bool UGridMovementComponent::CanMoveToTile(AGridTile* TargetTile, TArray<AGridTile*>& PotentialPath)
{
	if (TargetTile == CurrentTile) return false;

	ensure(PathFinder);

	TArray<AGridTile*> trash;
	GetAllReachableTiles(trash);

	if (trash.Contains(TargetTile))
	{
		PotentialPath.Reset();

		FGridPathFinderRequest Request;
		Request.GridActor = Grid;
		Request.Sender = GetOwner();
		Request.Start = CurrentTile;
		Request.Goal = TargetTile;
		Request.MaxCost = GetAvailableMovement();

		PathFinder->FindPath(Request, PotentialPath);

		return true;
	}

	return false;
}

bool UGridMovementComponent::CanPassTile(AGridTile* InTile)
{
	AActor* OccupyingUnit = InTile->GetOccupyingUnit();

	if (!OccupyingUnit)
	{
		//Tile is free and can be passed by anyone
		return true;
	}
	else
	{
		UGridMovementComponent* OtherGMC = OccupyingUnit->FindComponentByClass<UGridMovementComponent>();

		ensureMsgf(OtherGMC, TEXT("A tile was occupied by an actor without a grid movement component."));

		if (OtherGMC)
		{
			return OtherGMC->LetsThisPass(this);
		}
		else
		{
			return false;
		}
	}
}

bool UGridMovementComponent::LetsThisPass(UGridMovementComponent* InGMC)
{
	////TODO Change this to a more flexible approac

	//AGridUnit* OwnerAsUnit = Cast<AGridUnit>(GetOwner());
	//AGridUnit* OtherOwnerAsUnit = Cast<AGridUnit>(InGMC->GetOwner());

	////The GMC's owner is a grid unit that may allow passage based on alignment
	//if (OwnerAsUnit)
	//{
	//	TMap<EUnitAlignment, EUnitBlocking> PathBlocking;
	//	IGridEncounterPawn::Execute_GetPathBlocking(OwnerAsUnit, PathBlocking);

	//	return (*PathBlocking.Find(IGridEncounterPawn::Execute_GetAlignment(OtherOwnerAsUnit)) == EUnitBlocking::LetPass);
	//}

	//The GMC's owner is a different kind of actor, dont allow passage
	return false;
}

void UGridMovementComponent::GetAllReachableTiles(TArray<AGridTile*>& ReachableTiles)
{
	if (bReachableTilesCached && ReachableTilesCacheOrigin == CurrentTile && CacheMovementRadius == GetAvailableMovement())
	{
		ReachableTiles = CachedReachableTiles;
	}
	else
	{
		ensure(PathFinder);

		ReachableTiles.Reset();

		FGridPathFinderRequest Request;
		Request.GridActor = Grid;
		Request.Sender = GetOwner();
		Request.Start = CurrentTile;
		Request.Goal = nullptr;
		Request.MaxCost = GetAvailableMovement();

		PathFinder->GetReachableTiles(Request, ReachableTiles);

		CacheMovementRadius = GetAvailableMovement();
		bReachableTilesCached = true;
		ReachableTilesCacheOrigin = CurrentTile;
		CachedReachableTiles = ReachableTiles;
	}
}

void UGridMovementComponent::SetShowMovableArea(bool bShow)
{
	if (bShow)
	{
		SetShowMovableArea(false);
		GetAllReachableTiles(ShownReachableTiles);

		for (AGridTile* Tile : ShownReachableTiles)
		{
			//Tile->SetTileState(this, ETileState::ShowAsMovable, ETileStateLayers::HoverMovement);
		}
	}
	else
	{
		for (AGridTile* Tile : ShownReachableTiles)
		{
			//Tile->ResetTileState(this);
		}
		ShownReachableTiles.Reset();
	}
}

//void UGridMovementComponent::RefreshResources_OnEnd_Implementation()
//{
//	UsedMovement = 0;
//}


void UGridMovementComponent::OnLeaveTileCompleted(bool bSuccess)
{

	bAwaitingCallback = false;

	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Leave tile success"))

		DecrementAvailableMovement();
		//Set to travel to next tile
		CurrentTileLoc = CurrentTile->GetActorLocation();
		NextTile = PathToTravel.Pop();
		NextTileLoc = NextTile->GetActorLocation();

		bool bJumpingRequired = CurrentTile->ConnectedTiles.Find(NextTile)->bRequiresJump;
		MovementState = (bJumpingRequired ? PreJump : Walking);

		MovementDuration = ((CurrentTileLoc - NextTileLoc).Size() / MovementSpeed) * (bJumpingRequired ? 2.5f : 1.0f);

		TimePassed = 0.0f;
		CurrentTile = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Leave tile failure"))
		//The movement was interrupted, cancel the movement
		AbortMovement();
	}

}


int32 UGridMovementComponent::GetAvailableMovement()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	bool bFound;
	int32 val = ASC->GetGameplayAttributeValue(UGAS_UnitAttributeSet::GetAvailableMovementAttribute(), bFound);

	return (bFound ? val : 0.f);
}

void UGridMovementComponent::DecrementAvailableMovement()
{
	UGameplayEffect* DecrementEffect = NewObject<UGameplayEffect>(this, DecrementMovementEffectClass);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	ASC->ApplyGameplayEffectToSelf(DecrementEffect, 0.f, FGameplayEffectContextHandle());
}

void UGridMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Check whether motion needs to be progressed.
	if (bInMotion)
	{
		//Check whether motion to the next tile needs to be initiated
		if (CurrentTile)
		{
			//We're currently occupying a tile -> check path, initiate next motion
			if (PathToTravel.Num() > 0 && !bAwaitingCallback)
			{
				CurrentTile->TryLeaveTile(GetOwner(), std::bind(&UGridMovementComponent::OnLeaveTileCompleted, this, std::placeholders::_1));
				bAwaitingCallback = true;
			}
			else if (!bAwaitingCallback)
			{
				//We're on a tile and no tiles are in the path queue, end of motion
				bInMotion = false;
				CurrentTile->OccupyTile(GetOwner());
				MovementState = Idle;
				OnMovementCompleted();
			}
		}
		else
		{
			//We're currently in between tiles, control the motion
			TimePassed += DeltaTime;
			float BlendFactor = TimePassed / MovementDuration;

			if (BlendFactor < 1)
			{
				switch (MovementState)
				{
				case Walking: ProgressWalking(BlendFactor); break;
				case PreJump:	//all of these fall through to ProgressJumping
				case Jumping:
				case Falling:
				case Landing: ProgressJumping(BlendFactor); break;
				}
			}
			else
			{
				//Motion has finished
				CurrentTile = NextTile;
				CurrentTile->OccupyTile(GetOwner());
				MovementState = Walking;

				FVector TransitionLocation = NextTileLoc;// + FVector::UpVector * (Bounds.Z);
				//UKismetSystemLibrary::DrawDebugPoint(this, TransitionLocation, 20.f, FLinearColor::Blue, 3.f);
				GetWorld()->FindTeleportSpot(GetOwner(), TransitionLocation, GetOwner()->GetActorRotation());
				GetOwner()->SetActorLocation(TransitionLocation);

				//UKismetSystemLibrary::DrawDebugPoint(this, NextTileLoc, 50.f, FLinearColor::Red, 10.f );

				//Execute_SpendActionResources(this, 1);
				//CurrentTile->OnTilePassed.Broadcast(CurrentTile, GetOwner());
			}
		}
	}
}

bool UGridMovementComponent::IsComponentTickEnabled() const
{
	return bInMotion && !bPaused;
}

void UGridMovementComponent::ProgressWalking(float BlendValue)
{
	float Alpha = WalkCurve->GetFloatValue(BlendValue);
	FVector TransitionLocation = (1 - Alpha) * CurrentTileLoc + Alpha * NextTileLoc; // + FVector(0.0f, 0.0f, Bounds.Z);
	//UKismetSystemLibrary::DrawDebugPoint(this, TransitionLocation, 3.f, FLinearColor::Green, 3.f);

	GetWorld()->FindTeleportSpot(GetOwner(), TransitionLocation, GetOwner()->GetActorRotation());
	//UKismetSystemLibrary::DrawDebugPoint(this, TransitionLocation, 3.f, FLinearColor::Blue, 10.f);
	GetOwner()->SetActorLocation(TransitionLocation);
}

void UGridMovementComponent::ProgressJumping(float BlendValue)
{
	float AlphaXY = JumpCurve_Horizontal->GetFloatValue(BlendValue);
	float AlphaZ = JumpCurve_Vertical->GetFloatValue(BlendValue);
	float AlphaZDeriv = (JumpCurve_Vertical->GetFloatValue(BlendValue + 0.01f) - JumpCurve_Vertical->GetFloatValue(BlendValue)) / 0.01f;

	float ZValue = 0.0f;

	switch (MovementState)
	{
	case PreJump: ZValue = CurrentTileLoc.Z; break;
	case Jumping: ZValue = (1 - AlphaZ) * CurrentTileLoc.Z + AlphaZ * (UpperBound + Bounds.Z + JumpGap); break;
	case Falling: ZValue = (1 - AlphaZ) * NextTileLoc.Z + AlphaZ * (UpperBound + Bounds.Z + JumpGap); break;
	case Landing: ZValue = NextTileLoc.Z; break;
	}

	FVector TransitionLocationXY = (1 - AlphaXY) * CurrentTileLoc + AlphaXY * NextTileLoc;
	FVector TransitionLocation = FVector(TransitionLocationXY.X, TransitionLocationXY.Y, ZValue) + FVector::UpVector * Bounds.Z;
	//GetWorld()->FindTeleportSpot(GetOwner(), TransitionLocation, GetOwner()->GetActorRotation());
	GetOwner()->SetActorLocation(TransitionLocation);

	switch (MovementState)
	{
	case PreJump: if (AlphaZ != 0.f) MovementState = Jumping; break;
	case Jumping: if (AlphaZDeriv <= 0.f) MovementState = Falling; break;
	case Falling: if (AlphaZ <= 0.01f) MovementState = Landing; break;;
	default: break;
	}
}

void UGridMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();

	if (ensure(ASC))
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(MoveAbilityClass, 1, INDEX_NONE, this);
		MoveAbilityHandle = ASC->GiveAbility(Spec);
	}
}

void UGridMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGridMovementComponent, CurrentTile);
	DOREPLIFETIME(UGridMovementComponent, Grid);
}

//void UGridMovementComponent::RefreshResources_OnStart_Implementation()
//{
//	UsedMovement = 0;
//}

//void UGridMovementComponent::SetActionData_Implementation(UGridActionData_Base* InData)
//{
//	ActionData = InData;
//}
//
//void UGridMovementComponent::SpendActionResources_Implementation(int32 Amount)
//{
//	UsedMovement += Amount;
//}


void UGridMovementComponent::OnRegister()
{
	Super::OnRegister();

	GetOwner()->GetActorBounds(true, Origin, Bounds);
	PathFinder = NewObject<UGridMovementAgent>(this, TEXT("GridMovementComponent_PathFinder"));
	PathFinder->GMC = this;
}

void UGridMovementComponent::OnUnregister()
{
	Super::OnUnregister();

	PathFinder = nullptr;
}

void UGridMovementComponent::OnMovementCompleted()
{
	if (GetOwner())
	{
		//Notify the owner via gameplay event
		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
		if (AbilitySystemComponent != nullptr && IsValidChecked(AbilitySystemComponent))
		{
			FGameplayEventData Payload;
			Payload.OptionalObject = this;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(TEXT("GridMovement.OnComplete")), &Payload);
		}
	}

	OnComplete.Broadcast(this);
	OnComplete.Clear();
}
