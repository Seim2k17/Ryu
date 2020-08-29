// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RYUENUM_LedgePos.h"
#include "RYU2DENUM_Movement.h"
#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "RyuMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCanClimbLedgeStartedSignature, ERYULedgePosition2D,
                                            LedgePosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveModeChangedSignature, EPlayerMovement,
                                            PlayerMovement);

UCLASS()
class RYU_PROTOTYPE_API URyuMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    URyuMovementComponent();

    URyuMovementComponent(const class FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable)
    void ClimbDownLedgeFinished();

    bool DoJump(bool bReplayingMoves) override;

    UFUNCTION()
    int32 GetNormalMaxJumpCount();

    float GetSneakMultiplier();

    void IncreaseFallingVelocity();

    void JumpForward();

    void JumpUp();

    UFUNCTION(BlueprintCallable, Category = "Climbing")
    void ResetClimbingState();

    UFUNCTION(BlueprintCallable, Category = "CharacterMovement")
    void SetGravityScaleMaximum(float GravScale);

    UFUNCTION()
    void SetNormalMaxJumpCount(int32 MaxJumps);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetNoCollisionCharacterPrefs();

    void SetVelocityAfterJump(FJumpStartValues AfterJumpValues);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    virtual void InitializeComponent() override;

    virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode,
                                       uint8 PreviousCustomMode) override;

    virtual void PhysCustom(float deltaTime, int32 Iterations) override;

    virtual void PhysClimbingLedge(float deltaTime, int32 Iterations);

    virtual void PhysFallingLedge(float deltaTime, int32 Iterations);

    virtual void PhysClimbingLadder(float deltaTime, int32 Iterations);

    UFUNCTION()
    void SetAllowClimbUpTrue();

    UFUNCTION()
    void SetAllowClimbUpFalse();

    UFUNCTION(BlueprintCallable, Category = "Jumping")
    void StartLaunchCharacter();

    void ResetDoOnceClimbInput();

    /************************************************************************/
    /* MEMBER                                                               */
    /************************************************************************/

    // TODO MovementData separate from InputData / take care to NOT mix them
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Falling")
    float AddFallingMultiplierNumber;

    /**if 0 then Char->GravityMaximumStd (3) is used*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
    float GravityScaleMaximum;

    /** Jump Force added when pressing Jump from Stand (V(y) == 0*/
    UPROPERTY(VisibleAnywhere, Category = "Character Movement (Customization)")
    FVector JumpForce;

    /** Jump Force added when pressing Jump while Running*/
    UPROPERTY(VisibleAnywhere, Category = "Character Movement (Customization)")
    FVector JumpForceRun;

    UPROPERTY(VisibleAnywhere, Category = "Jumping")
    FVector JumpImpulse = FVector::ZeroVector;

    UPROPERTY(VisibleAnywhere, Category = "Jumping")
    FVector JumpUpImpulse = FVector::ZeroVector;

    /**Treshhold to activate VelocityAfterJumping, Y: vertical Velocity, Z: Falling Treshold */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
    FVector AfterJumpTreshold;

    /** Velocity added after Jump ends (if StartedVelocity > AfterJumpTreshold)*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
    FVector VelocityAfterJumping;

    /**Time allowed to jump when falling of a ledge*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
    float CoyoteTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
    bool CoyoteTimeActive;

    /*
	Time after a complete Jump is resetted to IDLE-State, this is a Fallback and can also be used for a short pause after landing. The Timer Should not be shorter than the JumpEnd-Animation !
	*/
    UPROPERTY(EditAnywhere, Category = "Jumping")
    float TimerEndJump = 0.3f;

    /*
	Time after which Velocity of the falling Character is increased by AddFallingMultiplierNumber * CurrentVelocity
	*/
    UPROPERTY(EditAnywhere, Category = "Falling")
    float FallingVelocityTimer = 0.05;

    UPROPERTY(EditAnywhere, Category = "Falling")
    float MaxFallingVelocityZ = -1500.f;

    FTimerHandle Timerhandle_CoyoteTime;

    UPROPERTY(BlueprintAssignable, Category = "Climbing")
    FOnCanClimbLedgeStartedSignature OnCanClimbLedge;

    UPROPERTY(BlueprintAssignable, Category = "Movement")
    FOnMoveModeChangedSignature OnMoveModeChanged;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Climbing")
    FVector ClimbDownStartPosition;

    float EndJumpTime = 0.0f;

    // How far will the character jump?
    UPROPERTY(VisibleAnywhere)
    float JumpForwardDistance = 50.0f;

    UPROPERTY(VisibleAnywhere, Category = "Jumping / Falling")
    float OverallJumpTime = 0.0f;

    float StartJumpTime = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Sneak")
    float SneakMultiplier = 0.5f;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
    float TreshholdYWalkRun;

private:
    int32 NormalMaxJumpCount;

    bool bDoStuffOnce;

    bool bDoOnceClimbInput;

    bool bClimbUpAllowed;
};
