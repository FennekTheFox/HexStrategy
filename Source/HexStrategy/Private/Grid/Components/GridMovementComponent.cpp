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
#include <GameFramework/CharacterMovementComponent.h>
#include <AIModule/Classes/AIController.h>



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

void UGridMovementComponent::SVR_AttachToGrid_Implementation(AGridActor* NewGrid /*= nullptr*/)
{
	AttachToGrid(NewGrid);
}

void UGridMovementComponent::AttachToGrid(AGridActor* NewGrid)
{
	ensure(GetOwner()->HasAuthority());

	if (NewGrid)
	{
		//If a grid has been passed, attach to it. If NewGrid is a nullptr, look for a grid
		Grid = NewGrid;
	}
	else
	{
		Grid = Cast<AGridActor>(UGameplayStatics::GetActorOfClass(this, AGridActor::StaticClass()));
	}

	if(!ensure(Grid))return;

	//Initialise the path finders reference to the grid
	NextTile = Grid->GetTileClosestToCoordinates(GetOwner()->GetActorLocation());
	NextTile->PreoccupyTile(GetOwner());
	CurrentTile = NextTile;

	AAIController* AIC = Cast<AAIController>(Cast<APawn>(GetOwner())->GetController());

	if (ensure(AIC))
	{
		AIC->ReceiveMoveCompleted.AddDynamic(this, &UGridMovementComponent::MovementCompletedCallback);
		RequestMoveToLocation(NextTile->WorldLocation);
	}

}

//void UGridMovementComponent::CL_AttachToGrid_Implementation(AGridActor* NewGrid /*= nullptr*/)
//{
//	//PathFinder->GridActor = NewGrid;
//}

void UGridMovementComponent::DetachFromGrid_Implementation(AGridActor* InGrid /*= nullptr*/)
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

void UGridMovementComponent::MoveToTile_Implementation(UGridTile* TargetTile)
{
	ensure(PathFinder);
	ensure(TargetTile);


	if (CurrentTile == TargetTile)
	{
		//Why move, when we're already there

		OnMovementCompleted();
		return;
	}

	TArray<UGridTile*>Path;

	FGridPathFinderRequest Request;
	Request.GridActor = Grid;
	Request.Sender = GetOwner();
	Request.Start = CurrentTile;
	Request.Goal = TargetTile;
	Request.MaxCost = GetAvailableMovement();

	if (PathFinder->FindPath(Request, Path))
	{
		Path.Remove(CurrentTile);

		if (Path.Num() != 0)
		{
			PathToTravel = Path;
			CurrentTile->TryLeaveTile(GetOwner(), std::bind(&UGridMovementComponent::OnLeaveTileCompleted, this, std::placeholders::_1));
		}
	}
}






bool UGridMovementComponent::GetPathTo(UGridTile* TargetTile, TArray<UGridTile*>& OutPath)
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
		ExecutingPathMovement = false;
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

bool UGridMovementComponent::CanMoveToTile(UGridTile* TargetTile, TArray<UGridTile*>& PotentialPath)
{
	if (TargetTile == CurrentTile) return false;

	ensure(PathFinder);

	TArray<UGridTile*> trash;
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

bool UGridMovementComponent::CanPassTile(UGridTile* InTile)
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

void UGridMovementComponent::GetAllReachableTiles(TArray<UGridTile*>& ReachableTiles)
{
	ReachableTiles.Reset();

	if(GetAvailableMovement() == 0 || !CurrentTile)
		return;

	if (bReachableTilesCached && 
		ReachableTilesCacheOrigin == CurrentTile && 
		CacheMovementRadius == GetAvailableMovement() &&
		CachedGridStateIndex == Grid->GetStateIndex())
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
		CachedGridStateIndex = Grid->GetStateIndex();
	}
}

void UGridMovementComponent::SetShowMovableArea(bool bShow)
{
	if (bShow)
	{
		SetShowMovableArea(false);
		GetAllReachableTiles(ShownReachableTiles);

		for (UGridTile* Tile : ShownReachableTiles)
		{
			//Tile->SetTileState(this, ETileState::ShowAsMovable, ETileStateLayers::HoverMovement);
		}
	}
	else
	{
		for (UGridTile* Tile : ShownReachableTiles)
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

	//Check if we could successfully leave the tile
	if (bSuccess)
	{
		CurrentTile->AbandonTile(GetOwner());
		//UE_LOG(LogTemp, Log, TEXT("Leave tile success"))

		//Reduce the available movement via a gameplay effect
		DecrementAvailableMovement();

		AAIController* AIC = Cast<AAIController>(Cast<APawn>(GetOwner())->GetController());
		NextTile = PathToTravel.Pop();
		
		if(ensure(AIC))
			AIC->MoveToLocation(NextTile->WorldLocation, 1, false, false);


		//Set to travel to next tile
		//CurrentTileLoc = CurrentTile->GetActorLocation();
		//NextTile = PathToTravel.Pop();
		//NextTileLoc = NextTile->GetActorLocation();

		bool bJumpingRequired = CurrentTile->ConnectedTiles.Find(NextTile->Coordinates)->bRequiresJump;
		MovementState = (bJumpingRequired ? PreJump : Walking);

		MovementDuration = ((CurrentTileLoc - NextTileLoc).Size() / MovementSpeed) * (bJumpingRequired ? 2.5f : 1.0f);

		TimePassed = 0.0f;
		CurrentTile = nullptr;
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("Leave tile failure"))
			//The movement was interrupted, cancel the movement
			AbortMovement();
	}

}


void UGridMovementComponent::MovementCompletedCallback(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{			
	if (Result == EPathFollowingResult::Success)
	{
		CurrentTile = NextTile;
		CurrentTile->OccupyTile(GetOwner());


		//We're currently occupying a tile -> check path, initiate next motion
		if (PathToTravel.Num() > 0)
		{
			CurrentTile->TryLeaveTile(GetOwner(), std::bind(&UGridMovementComponent::OnLeaveTileCompleted, this, std::placeholders::_1));
			bAwaitingCallback = true;
		}
		else
		{
			//We're on a tile and no tiles are in the path queue, end of motion
			ExecutingPathMovement = false;
			CurrentTile->OccupyTile(GetOwner());
			MovementState = Idle;
			OnMovementCompleted();
		}
	}


}

void UGridMovementComponent::RequestMoveToLocation(FVector Location)
{

	AAIController* AIC = Cast<AAIController>(Cast<APawn>(GetOwner())->GetController());
	//NextTile = PathToTravel.Pop();

	if (ensure(AIC))
	{
		AIC->MoveToLocation(NextTile->WorldLocation, 1, false, false);
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
	//if (!GetOwner()->HasAuthority() || bAwaitingCallback)
	//	return;

	////if (ExecutingPathMovement)
	////{
	////	if (CurrentTile)
	////	{
	////		if (PathToTravel.Num() > 0)
	////		{
	////			//Try to leave the current tile
	////			CurrentTile->TryLeaveTile(GetOwner(), std::bind(&UGridMovementComponent::OnLeaveTileCompleted, this, std::placeholders::_1));
	////			bAwaitingCallback = true;
	////		}
	////		else
	////		{
	////			//We're on a tile and the path is empty -> signal movement complete
	////			ExecutingPathMovement = false;
	////			CurrentTile->OccupyTile(GetOwner());
	////			OnMovementCompleted;
	////		}
	////	}
	////}
	////else
	////{

	////}

	////Check whether motion needs to be progressed.
	//if (ExecutingPathMovement)
	//{
	//	//Check whether motion to the next tile needs to be initiated
	//	if (CurrentTile)
	//	{
	//		//We're currently occupying a tile -> check path, initiate next motion
	//		if (PathToTravel.Num() > 0)
	//		{
	//			CurrentTile->TryLeaveTile(GetOwner(), std::bind(&UGridMovementComponent::OnLeaveTileCompleted, this, std::placeholders::_1));
	//			bAwaitingCallback = true;
	//		}
	//		else
	//		{
	//			//We're on a tile and no tiles are in the path queue, end of motion
	//			ExecutingPathMovement = false;
	//			CurrentTile->OccupyTile(GetOwner());
	//			MovementState = Idle;
	//			OnMovementCompleted();
	//		}
	//	}
	//	else
	//	{
	//		//We're currently in between tiles, control the motion
	//		TimePassed += DeltaTime;
	//		float BlendFactor = TimePassed / MovementDuration;

	//		if (BlendFactor < 1)
	//		{
	//			switch (MovementState)
	//			{
	//			case Walking: ProgressWalking(BlendFactor); break;
	//			case PreJump:	//all of these fall through to ProgressJumping
	//			case Jumping:
	//			case Falling:
	//			case Landing: ProgressJumping(BlendFactor); break;
	//			}
	//		}
	//		else
	//		{
	//			//Motion has finished
	//			CurrentTile = NextTile;
	//			CurrentTile->OccupyTile(GetOwner());
	//			MovementState = Walking;

	//			FVector TransitionLocation = NextTileLoc;// + FVector::UpVector * (Bounds.Z);
	//			//UKismetSystemLibrary::DrawDebugPoint(this, TransitionLocation, 20.f, FLinearColor::Blue, 3.f);
	//			GetWorld()->FindTeleportSpot(GetOwner(), TransitionLocation, GetOwner()->GetActorRotation());
	//			SVR_UpdateActorLocation(TransitionLocation);

	//			//UKismetSystemLibrary::DrawDebugPoint(this, NextTileLoc, 50.f, FLinearColor::Red, 10.f );

	//			//Execute_SpendActionResources(this, 1);
	//			//CurrentTile->OnTilePassed.Broadcast(CurrentTile, GetOwner());
	//		}
	//	}
	//}
}

bool UGridMovementComponent::IsComponentTickEnabled() const
{
	return ExecutingPathMovement && !bPaused;
}

void UGridMovementComponent::ProgressWalking(float BlendValue)
{
	//float Alpha = WalkCurve->GetFloatValue(BlendValue);
	//FVector TransitionLocation = (1 - Alpha) * CurrentTileLoc + Alpha * NextTileLoc; // + FVector(0.0f, 0.0f, Bounds.Z);
	////UKismetSystemLibrary::DrawDebugPoint(this, TransitionLocation, 3.f, FLinearColor::Green, 3.f);

	//GetWorld()->FindTeleportSpot(GetOwner(), TransitionLocation, GetOwner()->GetActorRotation());
	////UKismetSystemLibrary::DrawDebugPoint(this, TransitionLocation, 3.f, FLinearColor::Blue, 10.f);
	//SVR_UpdateActorLocation(TransitionLocation);
}

void UGridMovementComponent::ProgressJumping(float BlendValue)
{
//	float AlphaXY = JumpCurve_Horizontal->GetFloatValue(BlendValue);
//	float AlphaZ = JumpCurve_Vertical->GetFloatValue(BlendValue);
//	float AlphaZDeriv = (JumpCurve_Vertical->GetFloatValue(BlendValue + 0.01f) - JumpCurve_Vertical->GetFloatValue(BlendValue)) / 0.01f;
//
//	float ZValue = 0.0f;
//
//	switch (MovementState)
//	{
//	case PreJump: ZValue = CurrentTileLoc.Z; break;
//	case Jumping: ZValue = (1 - AlphaZ) * CurrentTileLoc.Z + AlphaZ * (UpperBound + Bounds.Z + JumpGap); break;
//	case Falling: ZValue = (1 - AlphaZ) * NextTileLoc.Z + AlphaZ * (UpperBound + Bounds.Z + JumpGap); break;
//	case Landing: ZValue = NextTileLoc.Z; break;
//	}
//
//	FVector TransitionLocationXY = (1 - AlphaXY) * CurrentTileLoc + AlphaXY * NextTileLoc;
//	FVector TransitionLocation = FVector(TransitionLocationXY.X, TransitionLocationXY.Y, ZValue) + FVector::UpVector * Bounds.Z;
//	//GetWorld()->FindTeleportSpot(GetOwner(), TransitionLocation, GetOwner()->GetActorRotation());
//	SVR_UpdateActorLocation(TransitionLocation);
//
//	switch (MovementState)
//	{
//	case PreJump: if (AlphaZ != 0.f) MovementState = Jumping; break;
//	case Jumping: if (AlphaZDeriv <= 0.f) MovementState = Falling; break;
//	case Falling: if (AlphaZ <= 0.01f) MovementState = Landing; break;;
//	default: break;
//	}
}

void UGridMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();

	if (ensure(ASC) && GetOwner()->HasAuthority())
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
		if (!GetOwner()->HasAuthority()) return;

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