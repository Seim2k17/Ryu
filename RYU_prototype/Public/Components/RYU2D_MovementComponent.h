// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/RYUENUM_LedgePos.h"
#include "RYU2D_MovementComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCanClimb2DLedgeStartedSignature, ERYULedgePosition, LedgePosition);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class RYU_PROTOTYPE_API URYU2D_MovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
/************************************************************************/
/* METHODS                                                              */
/************************************************************************/

public:

	URYU2D_MovementComponent();

	URYU2D_MovementComponent(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "CharacterMovement")
	void SetGravityScaleMaximum(float GravScale);

	bool DoJump(bool bReplayingMoves) override;

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void ResetClimbingState();

	UFUNCTION()
	void SetNormalMaxJumpCount(int32 MaxJumps);

	UFUNCTION()
	int32 GetNormalMaxJumpCount();

	UFUNCTION(BlueprintCallable)
	void ClimbDownLedgeFinished();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void PhysClimbingLedge(float deltaTime, int32 Iterations);

	virtual void PhysFallingLedge(float deltaTime, int32 Iterations);

	virtual void PhysClimbingLadder(float deltaTime, int32 Iterations);

	bool bClimbUpAllowed;

/************************************************************************/
/* MEMBER                                                               */
/************************************************************************/

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		float AddFallingMultiplierNumber;

	/**if 0 then Char->GravityMaximumStd (3) is used*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		float GravityScaleMaximum;

	/** Jump Force added when pressing Jump from Stand (V(y) == 0*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		FVector JumpForce;

	/** Jump Force added when pressing Jump while Running*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		FVector JumpForceRun;

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

	FTimerHandle Timerhandle_CoyoteTime;

	UPROPERTY(BlueprintAssignable, Category = "Climbing")
	FOnCanClimb2DLedgeStartedSignature OnCanClimbLedge;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Climbing")
	FVector ClimbDownStartPosition;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float TreshholdYWalkRun;

private:

	int32 NormalMaxJumpCount;

	bool bDoStuffOnce;
	
	
};
