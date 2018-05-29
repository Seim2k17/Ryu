// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUCustomizeMovementComponent.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include <stdexcept>


// Sets default values for this component's properties, derived from CharacterMovementComponent
URYUCustomizeMovementComponent::URYUCustomizeMovementComponent(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
	//mostly just a Data Component
	VelocityAfterJumping.X = 0.0f;
	VelocityAfterJumping.Y = 450.0f;
	VelocityAfterJumping.Z = 0.0f;
	

	AfterJumpTreshold.X = 0.0f;
	AfterJumpTreshold.Y = 100.0f;
	AfterJumpTreshold.Z = -950.0f;

	JumpForce.X = 0.0f;
	JumpForce.Y = 300.0f;
	JumpForce.Z = 0.0f;

	JumpForceRun.X = 0.0f;
	JumpForceRun.Y = 0.0f;
	JumpForceRun.Z = 0.0f;

	AddFallingMultiplierNumber = 0.05f;
	GravityScaleMaximum = 4.0f;

	CoyoteTime = 0.3;

	CoyoteTimeActive = true;

	

}


// Called when the game starts
void URYUCustomizeMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}



void URYUCustomizeMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


void URYUCustomizeMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);



	UE_LOG(LogTemp, Log, TEXT("CustomModeChanged to %d"));
		
	//throw std::logic_error("The method or operation is not implemented.");
}


void URYUCustomizeMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	//throw std::logic_error("The method or operation is not implemented.");
}

bool URYUCustomizeMovementComponent::DoJump(bool bReplayingMoves)
{
	//Test JumpInput

	//UE_LOG(LogTemp, Warning, TEXT("JumpButton Pressed"));

	if (CharacterOwner && CharacterOwner->CanJump() || (GetOwner()->GetWorldTimerManager().GetTimerRemaining(Timerhandle_CoyoteTime) > 0.0f))
	{
		// Don't jump if we can't move up/down.
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f )
		{
			Velocity.Z = JumpZVelocity;
			SetMovementMode(MOVE_Falling);
			return true;
		}
	}

	return false;
}

void URYUCustomizeMovementComponent::SetNormalMaxJumpCount(int32 MaxJumps)
{
	NormalMaxJumpCount = MaxJumps;
}


int32 URYUCustomizeMovementComponent::GetNormalMaxJumpCount()
{
	return NormalMaxJumpCount;
}

void URYUCustomizeMovementComponent::SetGravityScaleMaximum(float GravScale)
{
	GravityScaleMaximum = GravScale;
}

