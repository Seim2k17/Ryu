// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_MovementComponent.h"
#include "RYU2D_CharacterBase.h"
#include "RYU2D_CharacterPrince.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"





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

	UE_LOG(LogTemp, Log, TEXT("Movement Mode changed ..."));

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
		//No need here --> managed in ABP
		MyChar->GetSprite()->SetLooping(false);
		MyChar->GetSprite()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//MyChar->GetMesh()->SetEnableGravity(false);
		MyChar->GetCapsuleComponent()->SetEnableGravity(false);
		MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
		MyChar->RYUClimbingMode = ERYUClimbingMode::CLIMBUPLEDGE;
		UE_LOG(LogTemp, Log, TEXT("... to CLIMB-UP LEDGE"));
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
		//MyChar->GetMesh()->SetEnableGravity(false);
		MyChar->GetCapsuleComponent()->SetEnableGravity(false);
		UE_LOG(LogTemp, Log, TEXT("... to CLIMB-DOWN LEDGE"));
		break;
	}
	case ERYUClimbingMode::HANGONLEDGE:
	{
		UE_LOG(LogTemp, Log, TEXT("... to Hanging: Waiting!"));
		MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
		MyChar->RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
		MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyChar->SphereTracer->SetEnableGravity(false);
		//MyChar->GetMesh()->SetEnableGravity(false);
		MyChar->GetCapsuleComponent()->SetEnableGravity(false);
		break;
	}
	case ERYUClimbingMode::JUMPTOLEDGE:
	{
		UE_LOG(LogTemp,Log,TEXT("... to CustomJumpUp:"))
		
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
		MyChar->GetSprite()->SetEnableGravity(false);
		//NoMesh in 2D Dude
		//MyChar->GetMesh()->SetEnableGravity(false);
		MyChar->GetCapsuleComponent()->SetEnableGravity(false);
		UE_LOG(LogTemp, Log, TEXT("... to FLYING"));
		break;
	}

	default:
		break;
	}
}


void URYU2D_MovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	switch (CustomMovementMode)
	{
	case ERYUClimbingMode::CLIMBUPLEDGE:
		if (!bDoStuffOnce)
		{
			bDoStuffOnce = true;
			UE_LOG(LogTemp, Log, TEXT("I´m climbing up the ledge!"));
		}
		PhysClimbingLedge(deltaTime, Iterations);
		break;
	case ERYUClimbingMode::CLIMBDOWNLEDGE:
		if (!bDoStuffOnce)
		{
			bDoStuffOnce = true;
			UE_LOG(LogTemp, Log, TEXT("I´m climbing down the ledge!"));
		}
		PhysClimbingLedge(deltaTime, Iterations);
		break;
	case ERYUClimbingMode::HANGONLEDGE:
		//Just Hang around
		if (!bDoStuffOnce)
		{
			bDoStuffOnce = true;
			UE_LOG(LogTemp, Log, TEXT("Hanging!"));
		}
		break;
	case ERYUClimbingMode::FALLDOWNLEDGE:
		if (!bDoStuffOnce)
		{
			bDoStuffOnce = true;
			UE_LOG(LogTemp, Log, TEXT("I´m falling down the ledge!"));
		}
		PhysClimbingLedge(deltaTime, Iterations);
		//PhysFallingLedge(deltaTime, Iterations);
		break;
	case ERYUClimbingMode::JUMPTOLEDGE:
		if (!bDoStuffOnce)
		{
			bDoStuffOnce = true;
			UE_LOG(LogTemp, Log, TEXT("I´m Jumping to the ledge!"));
		}
	case ERYUClimbingMode::CLIMBLADDERUP:
		PhysClimbingLadder(deltaTime, Iterations);
		break;
	case ERYUClimbingMode::CLIMBLADDERDOWN:
		PhysClimbingLadder(deltaTime, Iterations);
		break;
	}
}


void URYU2D_MovementComponent::PhysClimbingLedge(float deltaTime, int32 Iterations)
{
	//TODo: updates the movement take modified walking state ! ATM FlyingState active

	/** Following is copypasted from CharacterMovementComponent::PhysFlying and ajdusted to Phyclimbing*/

	//RestorePreAdditiveRootMotionVelocity();


	//ApplyRootMotionToVelocity(deltaTime);

	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.Time < 1.f)
	{
		const FVector GravDir = FVector(0.f, 0.f, -1.f);
		const FVector VelDir = Velocity.GetSafeNormal();
		const float UpDown = GravDir | VelDir;

		bool bSteppedUp = false;
		if ((FMath::Abs(Hit.ImpactNormal.Z) < 0.2f) && (UpDown < 0.5f) && (UpDown > -0.2f) && CanStepUp(Hit))
		{
			float stepZ = UpdatedComponent->GetComponentLocation().Z;
			bSteppedUp = StepUp(GravDir, Adjusted * (1.f - Hit.Time), Hit);
			if (bSteppedUp)
			{
				OldLocation.Z = UpdatedComponent->GetComponentLocation().Z + (OldLocation.Z - stepZ);
			}
		}

		if (!bSteppedUp)
		{
			//adjust and try again
			HandleImpact(Hit, deltaTime, Adjusted);
			SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
		}
	}

	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}

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

	//** Princespecific Stuff // use jumping for climbing up ? --> maybe NO
	ARYU2D_CharacterPrince* MyChar = Cast<ARYU2D_CharacterPrince>(CharacterOwner);
	if (MyChar && (MyChar->RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPLEDGE))
	{
		MyChar->Climb(1.0f);
	}
	return false;
}

void URYU2D_MovementComponent::ResetClimbingState()
{
	ARYU2D_CharacterBase* MyChar = Cast<ARYU2D_CharacterBase>(CharacterOwner);

	if (MyChar)
	{
		//	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
		//UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
		UE_LOG(LogTemp, Log, TEXT("ClimbReset."));
		MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MyChar->SphereTracer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MyChar->SphereTracer->SetEnableGravity(true);
		MyChar->GetMesh()->SetEnableGravity(true);
		MyChar->GetCapsuleComponent()->SetEnableGravity(true);
		
		SetAllowClimbUpFalse();
		
		SetMovementMode(MOVE_Walking);

		if ((MyChar->PlayerMovement != EPlayerMovement::CANGRABLEDGE))
		{
			MyChar->PlayerMovement = EPlayerMovement::STAND;
		}

		if ((MyChar->RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPLEDGE) &&
			(MyChar->RYUClimbingMode != ERYUClimbingMode::CANCLIMBDOWNLEDGE) &&
			(MyChar->RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPANDDOWN))
		{
			MyChar->RYUClimbingMode = ERYUClimbingMode::NONE;
		}

		ResetDoOnceClimbInput();
		
	}
}

void URYU2D_MovementComponent::SetNormalMaxJumpCount(int32 MaxJumps)
{
	NormalMaxJumpCount = MaxJumps;
}

int32 URYU2D_MovementComponent::GetNormalMaxJumpCount()
{
	return 0;
}

void URYU2D_MovementComponent::ClimbDownLedgeFinished()
{
	ARYU2D_CharacterPrince* MyChar = Cast<ARYU2D_CharacterPrince>(CharacterOwner);
	MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
	MyChar->RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
	SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::HANGONLEDGE));
	SetAllowClimbUpTrue();
}

void URYU2D_MovementComponent::SetAllowClimbUpTrue()
{
	UE_LOG(LogTemp, Log, TEXT("bAlloClimbing: true"));
	ARYU2D_CharacterPrince* MyChar = Cast<ARYU2D_CharacterPrince>(CharacterOwner);
	bClimbUpAllowed = true;
	MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
	MyChar->RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
}


void URYU2D_MovementComponent::SetAllowClimbUpFalse()
{
	UE_LOG(LogTemp, Log, TEXT("bAlloClimbing: false"));
	bClimbUpAllowed = false;
}


void URYU2D_MovementComponent::ResetDoOnceClimbInput()
{
	bDoOnceClimbInput = false;
}