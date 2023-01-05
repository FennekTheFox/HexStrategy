#pragma once

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Grid/GridActor.h"
#include "Grid/PathFinders/GridPathFinder.h"
#include "Abilities/GameplayAbility.h"
#include <AIModule/Classes/AITypes.h>
#include <AIModule/Classes/Navigation/PathFollowingComponent.h>
//#include "IGridActionComponent.h"

#include "GridMovementComponent.generated.h"

class UCurveFloat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridMovementDelegate, class UGridMovementComponent*, GridMovementComponent);

UENUM(BlueprintType)
enum EGridMovementState
{
	Idle,
	Walking,
	PreJump,
	Jumping,
	Falling,
	Landing
};


UCLASS(ClassGroup = (Grid), Blueprintable, meta = (BlueprintSpawnableComponent))
class UGridMovementComponent : public UActorComponent//, public IGridActionComponent
{
	//friend class UGridAction_Move;
	//friend class UGridAttackComponent;

	GENERATED_BODY()

public:
	UGridMovementComponent();

	UFUNCTION(Server, reliable, BlueprintCallable)
		void AttachToGrid(AGridActor* NewGrid = nullptr);
	//UFUNCTION(Client, reliable, BlueprintCallable)
	//	void CL_AttachToGrid(AGridActor* NewGrid = nullptr);
	UFUNCTION(BlueprintCallable, Server, reliable)
		void DetachFromGrid(AGridActor* InGrid = nullptr);
	UFUNCTION(Server, reliable, BlueprintCallable)
		void MoveToTile(AGridTile* TargetTile);
	UFUNCTION(BlueprintCallable)
		bool GetPathTo(AGridTile* TargetTile, TArray<AGridTile*>& OutPath);
	UFUNCTION(BlueprintCallable)
		void AbortMovement();
	UFUNCTION(BlueprintCallable)
		void PauseMovement();
	UFUNCTION(BlueprintCallable)
		void ResumeMovement();
	UFUNCTION(BlueprintPure)
		bool CanMoveToTile(AGridTile* TargetTile, TArray<AGridTile*>& PotentialPath);
	UFUNCTION(BlueprintPure)
		bool CanPassTile(AGridTile* InTile);
	UFUNCTION(BlueprintPure)
		bool LetsThisPass(UGridMovementComponent* InGMC);
	UFUNCTION(BlueprintPure)
		void GetAllReachableTiles(TArray<AGridTile*>& ReachableTiles);
	UFUNCTION(BlueprintCallable)
		void SetShowMovableArea(bool bShow);


	////BEGIN IGridActionComponent
	//void CollectActions_Implementation(TArray<UGridActionBase*>& OutActions) override;
	//void RefreshResources_OnEnd_Implementation() override;
	//void SpendActionResources_Implementation(int32 Amount) override;
	////END IGridActionComponent


	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsComponentTickEnabled() const override;

	float GetMaxJumpDistance() { return FMath::Max(24.0f, MaxJumpHeight * 25.f); };

	void ProgressWalking(float BlendValue);
	void ProgressJumping(float BlendValue);


	//void SetActionData_Implementation(UGridActionData_Base* InData) override;


	//void RefreshResources_OnStart_Implementation() override;


	void BeginPlay() override;


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void OnLeaveTileCompleted(bool bSuccess);
	UFUNCTION()
		void MovementCompletedCallback(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	void RequestMoveToLocation(FVector Location);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings")
		float MovementSpeed = 300;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings", meta = (DisplayName = "Jump Height (1 = 25Units)"))
		int32 MaxJumpHeight = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings", meta = (DisplayName = "Maximum walkable slope (in deg)"))
		float MaxWalkableSlopeIncline = 30.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings", meta = (DisplayName = "Maximum slidable slope (in deg)"))
		float MaxSlidableSlopeDecline = 45.f;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings")
	//	int32 MovementRadius = 10;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings|Data")
	//	class UGridActionData_Base* ActionData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings|Curves")
		UCurveFloat* WalkCurve;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings|Curves")
		UCurveFloat* JumpCurve_Horizontal;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement Settings|Curves")
		UCurveFloat* JumpCurve_Vertical;




	UPROPERTY(BlueprintAssignable)
		FGridMovementDelegate OnComplete;

	UPROPERTY(BlueprintReadOnly, Category = "Movement Query")
		TEnumAsByte<EGridMovementState> MovementState = EGridMovementState::Idle;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated, Category = "Movement Query")
		AGridTile* CurrentTile;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated, Category = "Movement Query")
		AGridActor* Grid;
	UPROPERTY(BlueprintReadOnly, Category = "Movement Query")
		UGridMovementAgent* PathFinder;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TSubclassOf<class UGAS_UnitAbility> MoveAbilityClass;


private:
	//int32 UsedMovement = 0;
	int32 GetAvailableMovement();
	void DecrementAvailableMovement();
	UPROPERTY()
		TSubclassOf<UGameplayEffect> DecrementMovementEffectClass;

	FVector Bounds, Origin;
	bool ExecutingPathMovement = false;
	bool bPaused = false;
	bool bAwaitingCallback = false;
	float UpperBound = 0.f;
	TArray<AGridTile*> PathToTravel;
	AGridTile* NextTile;
	FVector NextTileLoc;
	FVector CurrentTileLoc;
	float MovementDuration;
	float TimePassed;
	TArray<AGridTile*> ShownReachableTiles;
	float JumpGap = 0.f;

	//TODO: CachedMovable needs to reset every time a unit on the grid moves. Callback from Occupy/Leave?
	bool bReachableTilesCached = false;
	UPROPERTY()
		AGridTile* ReachableTilesCacheOrigin = nullptr;
	UPROPERTY()
		TArray<AGridTile*> CachedReachableTiles;
	int32 CacheMovementRadius;

protected:
	void OnRegister() override;
	void OnUnregister() override;
	void OnMovementCompleted();

private:
	FGameplayAbilitySpecHandle MoveAbilityHandle;
};