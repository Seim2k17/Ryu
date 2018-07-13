// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_MovementComponent.h"
#include "RYU2D_CharacterBase.h"
#include "RYU2D_CharacterPrince.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"





URYU2D_MovementComponent::URYU2D_MovementComponent()
{

}

URYU2D_MovementComponent::URYU2D_MovementComponent(const class FObjectInitializer& ObjectInitializer)
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

	bDoStuffOnce = false;
}

void URYU2D_MovementComponent::BeginPlay()
{
	Super::BeginPlay();

}


void URYU2D_MovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


void URYU2D_MovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	ARYU2D_CharacterBase* MyChar = Cast<ARYU2D_CharacterBase>(CharacterOwner);

	bDoStuffOnce = false;

	switch (CustomMovementMode)
	{
	case ERYUClimbingMode::CLIMBUPLEDGE:
	{
		//	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
		//UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
		MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyChar->SphereTracer->SetEnableGravity(false);
		MyChar->GetMesh()->SetEnableGravity(false);
		MyChar->GetCapsuleComponent()->SetEnableGravity(false);
		MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
		MyChar->RYUClimbingMode = ERYUClimbingMode::CLIMBUPLEDGE;
		UE_LOG(LogTemp, Log, TEXT("Mode changed to CLIMB-UP LEDGE"));
		break;
	}
	case ERYUClimbingMode::CLIMBDOWNLEDGE:
	{
		//	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
		//UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
		ClimbDownStartPosition = MyChar->GetActorLocation();
		MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//MyChar->SphereTracer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//MyChar->SphereTracer->SetEnableGravity(false);
		MyChar->GetMesh()->SetEnableGravity(false);
		MyChar->GetCapsuleComponent()->SetEnableGravity(false);
		UE_LOG(LogTemp, Log, TEXT("Mode changed to CLIMB-DOWN LEDGE"));
		break;
	}
	case ERYUClimbingMode::HANGONLEDGE:
	{
		UE_LOG(LogTemp, Log, TEXT("Waiting!"));
		MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
		MyChar->RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
		MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyChar->SphereTracer->SetEnableGravity(false);
		MyChar->GetMesh()->SetEnableGravity(false);
		MyChar->GetCapsuleComponent()->SetEnableGravity(false);
		break;
	}

	default:
		break;
	}

	/**FallBack , just use Flying */
	switch (MovementMode)
	{
	case MOVE_Flying:
	{
		ARYU2D_CharacterBase* MyChar = Cast<ARYU2D_CharacterBase>(CharacterOwner);
		//	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
		//UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
		MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyChar->SphereTracer->SetEnableGravity(false);
		MyChar->GetMesh()->SetEnableGravity(false);
		MyChar->GetCapsuleComponent()->SetEnableGravity(false);
		UE_LOG(LogTemp, Log, TEXT("Mode changed to FLYING"));
		break;
	}

	default:
		break;
	}
}


void URYU2D_MovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{

}


void URYU2D_MovementComponent::PhysClimbingLedge(float deltaTime, int32 Iterations)
{

}


void URYU2D_MovementComponent::PhysFallingLedge(float deltaTime, int32 Iterations)
{

}


void URYU2D_MovementComponent::PhysClimbingLadder(float deltaTime, int32 Iterations)
{

}

void URYU2D_MovementComponent::SetGravityScaleMaximum(float GravScale)
{

}

bool URYU2D_MovementComponent::DoJump(bool bReplayingMoves)
{
	//Test JumpInput

	//UE_LOG(LogTemp, Warning, TEXT("JumpButton Pressed"));

	if (CharacterOwner && CharacterOwner->CanJump() || (GetOwner()->GetWorldTimerManager().GetTimerRemaining(Timerhandle_CoyoteTime) > 0.0f))
	{
		// Don't jump if we can't move up/down.
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
			Velocity.Z = JumpZVelocity;
			SetMovementMode(MOVE_Falling);
			return true;
		}
	}

	//** Princespecific Stuff
	ARYU2D_CharacterPrince* MyChar = Cast<ARYU2D_CharacterPrince>(CharacterOwner);
	if (MyChar && (MyChar->RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPLEDGE))
	{
		MyChar->Climb(1.0f);
	}
	return false;
}

void URYU2D_MovementComponent::ResetClimbingState()
{

}

void URYU2D_MovementComponent::SetNormalMaxJumpCount(int32 MaxJumps)
{

}

int32 URYU2D_MovementComponent::GetNormalMaxJumpCount()
{
	return 0;
}

void URYU2D_MovementComponent::ClimbDownLedgeFinished()
{

}

