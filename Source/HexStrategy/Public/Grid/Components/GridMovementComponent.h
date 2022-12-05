#pragma once

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Grid/GridActor.h"
//#include "IGridActionComponent.h"

#include "GridMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGridMovementDelegate);


class UGridMovementAgent;

UENUM()
enum EGridMovementState
{
	Idle,
	Walking,
	Jumping,
	Landing,
};


UCLASS(ClassGroup = (Grid), Blueprintable, meta = (BlueprintSpawnableComponent))
class UGridMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGridMovementComponent();

	UFUNCTION(BlueprintCallable)
		void AttachToGrid(AGridActor* NewGrid = nullptr);
	UFUNCTION(BlueprintCallable)
		bool MoveToTile(UPARAM(ref)AGridTile* TargetTile);
	UFUNCTION(BlueprintCallable)
		bool GetPathTo(UPARAM(ref)AGridTile* TargetTile, TArray<AGridTile*>& OutPath);
	UFUNCTION(BlueprintCallable)
		void AbortMovement();
	UFUNCTION(BlueprintCallable)
		void PauseMovement();
	UFUNCTION(BlueprintCallable)
		void ResumeMovement();
	UFUNCTION(BlueprintPure)
		bool CanMoveToTile(UPARAM(ref)AGridTile* TargetTile, TArray<AGridTile*>& PotentialPath);
	UFUNCTION(BlueprintPure)
		bool CanPassTile(AGridTile* InTile);
	UFUNCTION(BlueprintPure)
		bool LetsThisPass(UGridMovementComponent* InGMC);
	UFUNCTION(BlueprintPure)
		TArray<AGridTile*> GetAllReachableTiles(TArray<AGridTile*>& ReachableTiles);
	UFUNCTION(BlueprintCallable)
		void SetShowMovableArea(bool bShow);


	////BEGIN IGridActionComponent
	//void CollectActions(TArray<UGridActionBase*>& OutActions) override;
	////END IGridActionComponent


	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsComponentTickEnabled() const override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings")
		float MovementSpeed = 300;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings")
		int32 MovementRadius = 10;


	UPROPERTY(BlueprintAssignable)
		FGridMovementDelegate OnComplete;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EGridMovementState> MovementState = EGridMovementState::Idle;
	UPROPERTY(BlueprintReadOnly)
		AGridTile* CurrentTile;
	UPROPERTY(BlueprintReadOnly)
		AGridActor* Grid;
	UPROPERTY(BlueprintReadOnly)
		UGridMovementAgent* PathFinder;


private:
	bool bInMotion = false;
	bool bPaused = false;
	TArray<AGridTile*> PathToTravel;
	AGridTile* NextTile;
	FVector NextTileLoc;
	FVector CurrentTileLoc;
	float DistanceToTravel;
	float TimePassed;
	TArray<AGridTile*> ShownReachableTiles;

protected:
	void OnRegister() override;
	void OnUnregister() override;

};