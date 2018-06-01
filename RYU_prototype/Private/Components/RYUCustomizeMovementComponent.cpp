// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUCustomizeMovementComponent.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "RYUENUM_MovementMode.h"
#include "RYUCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
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

	switch (CustomMovementMode)
	{
		case ERYUMovementMode::CLIMBUPLEDGE:
		{
			ARYUCharacterBase* MyChar = Cast<ARYUCharacterBase>(CharacterOwner);
			//	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
			//UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
			MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MyChar->SphereTracer->SetEnableGravity(false);
			MyChar->GetMesh()->SetEnableGravity(false);
			MyChar->GetCapsuleComponent()->SetEnableGravity(false);
			UE_LOG(LogTemp, Log, TEXT("Mode changed to CLIMBLEDGE"));
			break;
		}
		case ERYUMovementMode::CLIMBDOWNLEDGE:
		{
			ARYUCharacterBase* MyChar = Cast<ARYUCharacterBase>(CharacterOwner);
			//	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
			//UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
			MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MyChar->SphereTracer->SetEnableGravity(false);
			MyChar->GetMesh()->SetEnableGravity(false);
			MyChar->GetCapsuleComponent()->SetEnableGravity(false);
			UE_LOG(LogTemp, Log, TEXT("Mode changed to CLIMBLEDGE"));
			break;
		}
		case ERYUMovementMode::HANGONLEDGE:
		{
			UE_LOG(LogTemp, Log, TEXT("Waiting!"));
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
		ARYUCharacterBase* MyChar = Cast<ARYUCharacterBase>(CharacterOwner);
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
	

	/*UE_LOG(LogTemp, Log, TEXT("CustomModeChanged to %d"));*/
		
	//throw std::logic_error("The method or operation is not implemented.");
}


void URYUCustomizeMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime,Iterations);

	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	switch (CustomMovementMode)
	{
	case ERYUMovementMode::CLIMBUPLEDGE:
		PhysClimbingLedge(deltaTime, Iterations);
		break;
	case ERYUMovementMode::CLIMBDOWNLEDGE:
		PhysClimbingLedge(deltaTime, Iterations);
		break;
	case ERYUMovementMode::HANGONLEDGE:
		//Just Hang around
		//PhysClimbingLedge(deltaTime, Iterations);
		UE_LOG(LogTemp, Log, TEXT("Hanging!"));
		break;
	case ERYUMovementMode::CLIMBLADDER:
		PhysClimbingLadder(deltaTime, Iterations);
		break;
	}
	//throw std::logic_error("The method or operation is not implemented.");
}


void URYUCustomizeMovementComponent::PhysClimbingLedge(float deltaTime, int32 Iterations)
{

	//TODo: updates the movement take modified walking state ! ATM FlyingState active
	UE_LOG(LogTemp, Log, TEXT("I´m climbing the ledge!"));

	/** Following is copypasted from CharacterMovementComponent::PhysFlying and ajdusted to Phyclimbing*/

	RestorePreAdditiveRootMotionVelocity();


	ApplyRootMotionToVelocity(deltaTime);

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


void URYUCustomizeMovementComponent::PhysClimbingLadder(float deltaTime, int32 Iterations)
{
	UE_LOG(LogTemp, Log, TEXT("I´m climbing the ladder!"));
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


void URYUCustomizeMovementComponent::ResetClimbingState()
{
	ARYUCharacterBase* MyChar = Cast<ARYUCharacterBase>(CharacterOwner);
	//	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
	//UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
	MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MyChar->SphereTracer->SetEnableGravity(true);
	MyChar->GetMesh()->SetEnableGravity(true);
	MyChar->GetCapsuleComponent()->SetEnableGravity(true);
	SetMovementMode(MOVE_Walking);
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

