// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuMoveRightAxisInputState.h"
#include <CoreMinimal.h>
#include <Components/TimelineComponent.h>
#include "RyuMainCharacter.generated.h"

class URyuTimelineComponent;
class URyuDebugComponent;
/**
 * WIP - V2 MainCharacter
DONE 1. Sort Methods and pack them to components
2. Implement State Machine with GangOfFour StatePattern 
 */
UCLASS()
class RYU_PROTOTYPE_API ARyuMainCharacter : public ARyuBaseCharacter
{
    GENERATED_BODY()

public:
    ARyuMainCharacter(const class FObjectInitializer& ObjectInitializer);

    bool CheckFlipOverlappedActor(UBoxComponent* ClimbingTrigger);

    void Climb();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ClimbLedgeFlipBookFinished();

    void HandleInput(ERyuInputState Input) override;

    void Jump() override;

    void PostInitializeComponents() override;

    void ResetCollisionAndGravity();

    void StopJumping() override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void TurnRunFlipBookFinished();

    void Tick(float DeltaTime) override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetMoveRightInput();

    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetMoveUpInput();

    bool GetSneakActive();

protected:
    // really protected ?
    void BeginPlay() override;

    void JumpUpOrDown(float Val, FVector StartJumpPosition);

    // BP exposable ?
    // UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveRight(float Val);

    void MoveUp(float Value);

    UFUNCTION()
    void HandleSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                          AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                          int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult);

    UFUNCTION()
    void HandleSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void SneakPressed();

    UFUNCTION()
    void SneakReleased();

    UFUNCTION()
    void SprintPressed();

    UFUNCTION()
    void SprintReleased();

    void UpdateCharacter();

    UPROPERTY(VisibleAnywhere, Category = "Components")
    URyuTimelineComponent* RyuTimelineComponent;

    URyuClimbingComponent* RyuClimbingComponent;

private:
    void InitializeCharacterValues();

    float MoveRightKeyStatus();

public:
    // most need to be moved in components

private:
    float fDeltaSeconds;

    float MoveRightInput;

    float MoveUpInput;

    bool bSneakIsPressed;

    float SneakMultiplierValue;

    ERyuMoveRightAxisInputState MoveRightAxisState;
};

/*
protected:
    
	 * Character FSM
	 * Note: Do not define other members below this point, this protected block is reserved for CSM
	 
    virtual void TickStates(float DeltaTime); - called in Tick 

    virtual void OnSpawningEnter();
    virtual void OnSpawningTick(float DeltaTime);
    virtual void OnSpawningExit();
    virtual void TransitionSpawningToIdle();

    virtual void OnIdleEnter();
    virtual void OnIdleTick(float DeltaTime);
    virtual void OnIdleExit();
    virtual void TransitionIdleToWalking();
    virtual void TransitionIdleToAttacking();
    virtual void TransitionIdleToDying();

    virtual void OnWalkingEnter();
    virtual void OnWalkingTick(float DeltaTime);
    virtual void OnWalkingExit();
    virtual void TransitionWalkingToIdle();
    virtual void TransitionWalkingToRunning();
    virtual void TransitionWalkingToAttacking();
    virtual void TransitionWalkingToDying();

    virtual void OnRunningEnter();
    virtual void OnRunningTick(float DeltaTime);
    virtual void OnRunningExit();
    virtual void TransitionRunningToWalking();
    virtual void TransitionRunningToAttacking();
    virtual void TransitionRunningToDying();

    virtual void OnAttackingEnter();
    virtual void OnAttackingTick(float DeltaTime);
    virtual void OnAttackingExit();
    virtual void TransitionAttackingToIdle();

    virtual void OnDyingEnter();
    virtual void OnDyingTick(float DeltaTime);
    virtual void OnDyingExit();
    virtual void TransitionDyingToDead();

    virtual void OnDeadEnter();
    virtual void OnDeadTick(float DeltaTime);


*/
