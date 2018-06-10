// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUCustomizeMovementComponent.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "RYUENUM_MovementMode.h"
#include "RYUCharacterBase.h"
#include "RYUCharacterIchi.h"
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

	ARYUCharacterBase* MyChar = Cast<ARYUCharacterBase>(CharacterOwner);

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
			MyChar->RYUMovement = ERYUMovementMode::CLIMB;
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
			MyChar->RYUMovement = ERYUMovementMode::CLIMB;
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

//RootBonePosition
// 	ARYUCharacterBase* MyChar = Cast<ARYUCharacterBase>(CharacterOwner);
// 	FVector RootBone = MyChar->GetMesh()->GetBoneLocation("root");
// 	UE_LOG(LogTemp, Log, TEXT("RootBone: %s"), *RootBone.ToString());
//MeshPosition
//  	FVector MeshPosi = MyChar->GetMesh()->GetPosition();
//  	UE_LOG(LogTemp, Log, TEXT("RootBone: %s"), *MeshPosi.ToString());

	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	switch (CustomMovementMode)
	{
	case ERYUClimbingMode::CLIMBUPLEDGE:
		UE_LOG(LogTemp, Log, TEXT("I´m climbing up the ledge!"));
		PhysClimbingLedge(deltaTime, Iterations);
		break;
	case ERYUClimbingMode::CLIMBDOWNLEDGE:
		UE_LOG(LogTemp, Log, TEXT("I´m climbing down the ledge!"));
		PhysClimbingLedge(deltaTime, Iterations);
		break;
	case ERYUClimbingMode::HANGONLEDGE:
		//Just Hang around
		//PhysClimbingLedge(deltaTime, Iterations);
		UE_LOG(LogTemp, Log, TEXT("Hanging!"));
		break;
	case ERYUClimbingMode::FALLDOWNLEDGE:
		UE_LOG(LogTemp, Log, TEXT("I´m falling down the ledge!"));
		PhysClimbingLedge(deltaTime, Iterations);
		//PhysFallingLedge(deltaTime, Iterations);
		break;
	case ERYUClimbingMode::CLIMBLADDERUP:
		PhysClimbingLadder(deltaTime, Iterations);
		break;
	case ERYUClimbingMode::CLIMBLADDERDOWN:
		PhysClimbingLadder(deltaTime, Iterations);
		break;
	}
	//throw std::logic_error("The method or operation is not implemented.");
}


void URYUCustomizeMovementComponent::PhysClimbingLedge(float deltaTime, int32 Iterations)
{

	//TODo: updates the movement take modified walking state ! ATM FlyingState active

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


void URYUCustomizeMovementComponent::PhysFallingLedge(float deltaTime, int32 Iterations)
{
	//workaround for NoRootMotion 
// 	ARYUCharacterBase* MyChar = Cast<ARYUCharacterBase>(CharacterOwner);
// 	FVector MeshPosition = MyChar->GetMesh()->GetComponentLocation();
// 	MyChar->SetActorLocation(MeshPosition);


	//RootMotionactive
// 	if (MyChar->IsPlayingRootMotion())
// 	{
// 		//UE_LOG(LogTemp, Log, TEXT("ROOTMOTIONANI IS PLAYING."));
// 	}
// 	else
// 	{
// 		//UE_LOG(LogTemp, Log, TEXT("THE ANI HAS NO ROOTMOTION."));
// 	}


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
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
			Velocity.Z = JumpZVelocity;
			SetMovementMode(MOVE_Falling);
			return true;
		}
	}

	ARYUCharacterIchi* MyChar = Cast<ARYUCharacterIchi>(CharacterOwner);
	if (MyChar && (MyChar->RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPLEDGE))
	{
		MyChar->Climb(1.0f);
	}
	return false;
}


void URYUCustomizeMovementComponent::ResetClimbingState()
{
	ARYUCharacterBase* MyChar = Cast<ARYUCharacterBase>(CharacterOwner);

	if (MyChar)
	{
		//	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
		//UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
		MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MyChar->SphereTracer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MyChar->SphereTracer->SetEnableGravity(true);
		MyChar->GetMesh()->SetEnableGravity(true);
		MyChar->GetCapsuleComponent()->SetEnableGravity(true);
		ARYUCharacterIchi* MyRYUChar = Cast<ARYUCharacterIchi>(MyChar);
		if (MyRYUChar)
		{
			MyRYUChar->ToggleAllowClimbUp();
		}
		SetMovementMode(MOVE_Walking);
		if ((MyChar->RYUMovement != ERYUMovementMode::CANTRACELEDGE) &&
			(MyChar->RYUMovement != ERYUMovementMode::CANGRABLEDGE) &&
			(MyChar->RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPLEDGE) &&
			(MyChar->RYUClimbingMode != ERYUClimbingMode::CANCLIMBDOWNLEDGE))
		{
			MyChar->RYUMovement = ERYUMovementMode::STAND;
			MyChar->RYUClimbingMode = ERYUClimbingMode::NONE;
		}
	}
}

void URYUCustomizeMovementComponent::SetNormalMaxJumpCount(int32 MaxJumps)
{
	NormalMaxJumpCount = MaxJumps;
}


int32 URYUCustomizeMovementComponent::GetNormalMaxJumpCount()
{
	return NormalMaxJumpCount;
}


void URYUCustomizeMovementComponent::ClimbDownLedgeFinished()
{
	ARYUCharacterIchi* MyChar = Cast<ARYUCharacterIchi>(CharacterOwner);
	MyChar->RYUMovement = ERYUMovementMode::CLIMB;
	MyChar->RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
	MyChar->CustMovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::HANGONLEDGE));
	MyChar->ToggleAllowClimbUp();
	

}

void URYUCustomizeMovementComponent::SetGravityScaleMaximum(float GravScale)
{
	GravityScaleMaximum = GravScale;
}

