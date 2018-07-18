﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_MainCharacterZD.h"
#include "RYU2D_MovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/RYU2D_MovementComponent.h"
#include "Components/RYU2D_CurveDataComponent.h"
#include "Curves/CurveVector.h"



ARYU2D_MainCharacterZD::ARYU2D_MainCharacterZD(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URYU2D_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bDebugOutputActive = true;

	Curve2DComponent = CreateDefaultSubobject<URYU2D_CurveDataComponent>(TEXT("FloatCurves"));

	InitializeCharacterValues();
}

void ARYU2D_MainCharacterZD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//@ToDo: take closer look when CustomMovement will be impl.
	MovementComp = Cast<URYU2D_MovementComponent>(GetCharacterMovement());
}

void ARYU2D_MainCharacterZD::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

}

void ARYU2D_MainCharacterZD::InitializeCharacterValues()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;


	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	bPlayTurnAni = false;
	bLookRight = true;

	bStartedNoLoopAnimation = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(50.0f);
	GetCapsuleComponent()->SetCapsuleRadius(20.0f);

	/*
	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	SphereCollider->InitSphereRadius(45.0f);
	SphereCollider->SetRelativeLocation(FVector(25.0f, 0.0f, 0.0f));
	*/


	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 900.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	PlayerMovement = EPlayerMovement::STAND;
	CharAnimation2DState = ERYU2DAnimationState::IDLE;

	CurrentTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	onTimelineCallbackFloat.BindUFunction(this, FName("TimelineCallbackFloat"));
	onTimelineCallbackVector.BindUFunction(this, FName("TimelineCallbackVector"));
	onTimelineFinishedCallback.BindUFunction(this, FName("TimelineFinishedCallback"));
}


void ARYU2D_MainCharacterZD::BeginPlay()
{
	Super::BeginPlay();

	//do we still need it
	//GetSprite()->OnFinishedPlaying.AddDynamic(this, &ARYU2D_CharacterPrince::FlipbookFinishedPlaying);

	SphereTracer->OnComponentBeginOverlap.AddDynamic(this, &ARYU2D_MainCharacterZD::HandleSphereColliderBeginOverlap);
	SphereTracer->OnComponentEndOverlap.AddDynamic(this, &ARYU2D_MainCharacterZD::HandleSphereColliderEndOverlap);
}


void ARYU2D_MainCharacterZD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Temp save for Timelinestuff
	fDeltaSeconds = DeltaTime;

	if (bDebugOutputActive)
	{
		DrawDebugInfosOnScreen();
	}

	//** due its better for complexicity AND clarity we do most of the ABP_Transition_Logic here in c++ 
	UpdateCharacter();
}

void ARYU2D_MainCharacterZD::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARYU2D_MainCharacterZD::MoveRight);
	PlayerInputComponent->BindAxis("Climb", this, &ARYU2D_MainCharacterZD::MoveUp);
}

void ARYU2D_MainCharacterZD::Jump()
{
	bPressedJump = true;
}

void ARYU2D_MainCharacterZD::StopJumping()
{
	bPressedJump = false;

	Super::ResetJumpState();
}


//Move Left or Right
void ARYU2D_MainCharacterZD::MoveRight(float Val)
{
	
	MoveRightInput = Val;

	if ((PlayerMovement != EPlayerMovement::BEGINRUN) &&
		(PlayerMovement != EPlayerMovement::ENDRUN) &&
		(PlayerMovement != EPlayerMovement::STARTTURN) &&
		(PlayerMovement != EPlayerMovement::ENDTURN))
	{

		if ((bLookRight && Val > 0) || (!bLookRight && Val < 0))
		{
			AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val);
		}
	}
}

//Move Up or Down
void ARYU2D_MainCharacterZD::MoveUp(float Value)
{
	MoveUpInput = Value;
}

void ARYU2D_MainCharacterZD::Climb()
{
	switch (RYUClimbingMode)
	{
	case ERYUClimbingMode::HANGONLEDGE:
		if (MoveUpInput < 0)
		{
			RYUClimbingMode = ERYUClimbingMode::LETGOLEDGE;
		}
		else if (MoveUpInput > 0)
		{
			/*Without a Timeline but we need to adjust the Pivotpoint in EVERY Frame this SUCKS! maybe i can automatisize it when making my own animatins in PS (like we did in Visionaire with a positioning expotfile?)*/
			/*
			Curve2DComponent->ClimbUpStartTimelineLocation = GetActorLocation();
			Curve2DComponent->ClimbUpEndTimelineLocation = FVector(Curve2DComponent->ClimbUpStartTimelineLocation.X + Curve2DComponent->ClimbUpOffsetX, Curve2DComponent->ClimbUpStartTimelineLocation.Y,
				Curve2DComponent->ClimbUpStartTimelineLocation.Z + Curve2DComponent->ClimbUpOffsetZ);

			SetCurrentTimelineParamsFloat(Curve2DComponent->ClimbUpFloatCurveX, Curve2DComponent->ClimbUpFloatCurveZ, false, true);
			PlayTimeline();
			*/
			MovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::CLIMBUPLEDGE));
			RYUClimbingMode = ERYUClimbingMode::CLIMBUPLEDGE;
		}
		break;
	default:
		break;
	}
}

//* is called in MoveRight(Val), not in Tick anymore, but as MoveRight is bound to an axis this Ticks also
void ARYU2D_MainCharacterZD::UpdateCharacter()
{
	currA = GetCharacterMovement()->GetCurrentAcceleration();
	currV = this->GetVelocity();


	//** ABP-TRANSITION-RULES *******//
	//** TransitionRules for the ABP, moved it completely to c++ due clarity and complexicity reason
	if (currV.Z < 0)
	{
		PlayerMovement = EPlayerMovement::FALLING;
	}
	else
	{
		//@ToDo curV.Z > 0 (was auch immer das heisst)
		//
		if ((bLookRight && MoveRightInput < 0) || (!bLookRight && MoveRightInput > 0))
		{
			PlayerMovement = EPlayerMovement::STARTTURN;
		}

		switch (PlayerMovement)
		{
		case EPlayerMovement::STAND:
			if ((bLookRight && (currV.X > 0)) ||
				(!bLookRight && (currV.X < 0)))
			{
				PlayerMovement = EPlayerMovement::BEGINRUN;
				break;
			}

			CheckJumpUpState();

			//return;
			break;
		case EPlayerMovement::BEGINRUN:
			if (MoveRightInput == 0)
			{
				PlayerMovement = EPlayerMovement::ENDRUN;
			}
			else
			{
				PlayerMovement = EPlayerMovement::RUN;
			}
			//return;
			break;
		case EPlayerMovement::RUN:
			if (MoveRightInput == 0)
				PlayerMovement = EPlayerMovement::ENDRUN;
			//return; 
			break;
		case EPlayerMovement::ENDTURN:
			PlayerMovement = EPlayerMovement::STAND;
			//return;
			break;
		case EPlayerMovement::FALLING:
			PlayerMovement = EPlayerMovement::STANDUP;
			break;
			//return;
		case EPlayerMovement::CLIMBING:
			Climb();
		case EPlayerMovement::CANGRABLEDGE:
			CheckJumpUpState();
			break;
		default:
			break;
		}
	}
}


void ARYU2D_MainCharacterZD::CheckJumpUpState()
{
	
	if (MoveUpInput > 0)
	{
		/*
		Curve2DComponent->ClimbUpStartTimelineLocation = GetActorLocation();
		Curve2DComponent->ClimbUpEndTimelineLocation = FVector(Curve2DComponent->ClimbUpStartTimelineLocation.X, Curve2DComponent->ClimbUpStartTimelineLocation.Y,
			Curve2DComponent->ClimbUpStartTimelineLocation.Z + Curve2DComponent->ClimbUpOffsetZ);
		*/
		switch (RYUClimbingMode)
		{
		case ERYUClimbingMode::NONE:
			PlayerMovement = EPlayerMovement::JUMPUP;
			//** Initialize the Start End Endpoints 
			//MovementComp->SetMovementMode(MOVE_Flying);

			//MovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::HANGONLEDGE));
			//SetCurrentTimelineParamsFloat(Curve2DComponent->JumpUpFloatCurve, nullptr, false, true);
			//PlayTimeline();
			MovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::JUMPTOLEDGE));
			break;
		case ERYUClimbingMode::CANCLIMBUPLEDGE:
		{
			PlayerMovement = EPlayerMovement::CLIMBING;
			//SetCurrentTimelineParamsFloat(Curve2DComponent->JumpUpAndHangFloatCurve, nullptr, false, true);
			//PlayTimeline();
			FVector PosChar = FVector(ClimbUpStandDownPosition.X, ClimbUpStandDownPosition.Y, ClimbUpStandDownPosition.Z + 50);
			SetActorLocation(PosChar);
			MovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::JUMPTOLEDGE));
			break;
		}
			
		case ERYUClimbingMode::CANCLIMBUPANDDOWN:
			PlayerMovement = EPlayerMovement::CLIMBING;
			break;
		case ERYUClimbingMode::CANENTERLADDER:
			//@ToDo
			break;
		default:
			break;
		}
	}
}

void ARYU2D_MainCharacterZD::HandleSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void ARYU2D_MainCharacterZD::HandleSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ARYU2D_MainCharacterZD::TimelineCallbackFloat(float val)
{
	// This function is called for every tick in the timeline.
	//UE_LOG(LogTemp, Log, TEXT("Here incr. z-Location: %s"), *FString::SanitizeFloat(val));

	FVector StartTLLocation;
	FVector EndTLLocation;

	StartTLLocation = Curve2DComponent->ClimbUpStartTimelineLocation;
	EndTLLocation = Curve2DComponent->ClimbUpEndTimelineLocation;

	//Switch PMove needable ? ? ? or are all timelines handled the same ?
	switch (PlayerMovement)
	{
	case EPlayerMovement::JUMPUP:
		//@ToDo: look for ClimbUp vs. Down etc // see mix from 3D LedgeClimbing but wo Rootmotion ...
		
		break;
	default:
		break;
	}

	SetActorLocation(FMath::Lerp(StartTLLocation, EndTLLocation, val));
	//AddActorLocalOffset(FVector(0,0,CurrentTimeline->));
}

void ARYU2D_MainCharacterZD::TimelineCallbackVector(FVector Vec)
{
	// This function is called for every tick in the timeline.
	//UE_LOG(LogTemp, Log, TEXT("Here incr. z-Location: %s"), *FString::SanitizeFloat(val));

	FVector StartTLLocation;
	FVector EndTLLocation;

	StartTLLocation = Curve2DComponent->ClimbUpStartTimelineLocation;
	EndTLLocation = Curve2DComponent->ClimbUpEndTimelineLocation;

	//Switch PMove needable ? ? ? or are all timelines handled the same ?
	switch (PlayerMovement)
	{
	case EPlayerMovement::JUMPUP:
		//@ToDo: look for ClimbUp vs. Down etc // see mix from 3D LedgeClimbing but wo Rootmotion ...

		break;
	default:
		break;
	}

	SetActorLocation(FMath::Lerp(StartTLLocation, EndTLLocation, Vec));
	//AddActorLocalOffset(FVector(0,0,CurrentTimeline->));
}

void ARYU2D_MainCharacterZD::TimelineFinishedCallback()
{
	// This function is called when the timeline finishes playing.

	//SetActorLocation(Curve2DComponent->ClimbUpEndTimelineLocation);

	//we use this to set back the movementmode now we go to the fun stuff the real climbing !

	switch (PlayerMovement)
	{
	case EPlayerMovement::JUMPUP:
		MovementComp->SetMovementMode(MOVE_Walking);
		break;
	default:
		break;
	}

	switch (RYUClimbingMode)
	{
	case ERYUClimbingMode::NONE:
		break;
	case ERYUClimbingMode::CANCLIMBUPLEDGE:
		break;
	case ERYUClimbingMode::CANCLIMBDOWNLEDGE:
		break;
	case ERYUClimbingMode::CANCLIMBUPANDDOWN:
		break;
	case ERYUClimbingMode::JUMPTOLEDGE:
		break;
	case ERYUClimbingMode::CLIMBDOWNLEDGE:
		break;
	case ERYUClimbingMode::CLIMBUPLEDGE:
		MovementComp->SetMovementMode(MOVE_Walking);
		break;
	case ERYUClimbingMode::FALLDOWNLEDGE:
		break;
	case ERYUClimbingMode::HANGONLEDGE:
		break;
	case ERYUClimbingMode::LETGOLEDGE:
		break;
	case ERYUClimbingMode::CANENTERLADDER:
		break;
	case ERYUClimbingMode::CLIMBLADDERUP:
		break;
	case ERYUClimbingMode::CLIMBLADDERDOWN:
		break;
	default:
		break;
	}
}

void ARYU2D_MainCharacterZD::PlayTimeline()
{
	if (CurrentTimeline != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Play a timeline from Start"));
		CurrentTimeline->PlayFromStart();
	}
}

void ARYU2D_MainCharacterZD::SetCurrentTimelineParamsFloat(UCurveFloat* FloatCurveX, UCurveFloat* FloatCurveZ,  bool TimelineIsLooping, bool IgnoreTimeDilation)
{
	if (FloatCurveX)
	{
		UE_LOG(LogTemp, Log, TEXT("Set TimeLine Params for Timeline X %s"), *FloatCurveX->GetName());
		CurrentTimeline->AddInterpFloat(FloatCurveX, onTimelineCallbackFloat, FName("MovementX"));
	}

	if (FloatCurveZ)
	{

		UE_LOG(LogTemp, Log, TEXT("Set TimeLine Params for Timeline Z %s"), *FloatCurveZ->GetName());
		//Add Float curve to the timeline and bind it to the interpfunction´s delegate
		//3rd Parameter = floatValue, Propertyname, Bind all Stuff

		CurrentTimeline->AddInterpFloat(FloatCurveZ, onTimelineCallbackFloat, FName("MovementZ"));
	}

		CurrentTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		// Timeline Settings
		CurrentTimeline->SetLooping(TimelineIsLooping);
		//false = it moves as the global Time Dileation
		CurrentTimeline->SetIgnoreTimeDilation(IgnoreTimeDilation);

}

void ARYU2D_MainCharacterZD::SetCurrentTimelineParamsVector(UCurveVector* VectorCurve, bool TimelineIsLooping, bool IgnoreTimeDilation)
{
	if (VectorCurve)
	{
		UE_LOG(LogTemp, Log, TEXT("Set TimeLine Params for Timeline %s"), *VectorCurve->GetName());
		//Add Float curve to the timeline and bind it to the interpfunction´s delegate
		//3rd Parameter = floatValue, Propertyname, Bind all Stuff
		CurrentTimeline->AddInterpVector(VectorCurve, onTimelineCallbackVector, FName("MovementVec"));
		CurrentTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		// Timeline Settings
		CurrentTimeline->SetLooping(TimelineIsLooping);
		//false = it moves as the global Time Dileation
		CurrentTimeline->SetIgnoreTimeDilation(IgnoreTimeDilation);
	}
}

void ARYU2D_MainCharacterZD::CanClimbUp(float Val, FVector StartClimbUpPosition)
{

}

void ARYU2D_MainCharacterZD::CanClimbDown(float Val)
{

}

void ARYU2D_MainCharacterZD::CanClimbUpAndDown(float Val, FVector StartClimbPosition)
{

}

void ARYU2D_MainCharacterZD::HangOnLedgeAndClimb(float Val)
{

}

void ARYU2D_MainCharacterZD::CanGrabLedges(float Val)
{

}

void ARYU2D_MainCharacterZD::JumpUpOrDown(float Val, FVector StartJumpPosition)
{

}

/** Getter and Setter*/

FVector ARYU2D_MainCharacterZD::GetCurrentVelocity()
{
	return currV;
}

FVector ARYU2D_MainCharacterZD::GetCurrentAcceleration()
{
	return currA;
}

bool ARYU2D_MainCharacterZD::GetIsLookingRight()
{
	return bLookRight;
}

void ARYU2D_MainCharacterZD::SetLookRight()
{
	if (currV.X > 0)
	{
		bLookRight = true;
	}
	else if (currV.X < 0)
	{
		bLookRight = false;
	}

}

void ARYU2D_MainCharacterZD::TurnFlipBookFinished()
{
	//float TravelDirection = currV.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		//if (TravelDirection < 0.0f)
		if (bLookRight)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
			CameraBoom->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
		}
		//else if (TravelDirection > 0.0f)
		else
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
			CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
		}
	}

	bLookRight = !bLookRight;

	CheckOverlappingActors();
}

void ARYU2D_MainCharacterZD::ClimbUpFlipBookFinished()
{
	UE_LOG(LogTemp, Log, TEXT("Call From Notify: ClimbUpFlippbookFinished"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereTracer->SetEnableGravity(true);
	GetCapsuleComponent()->SetEnableGravity(true);
	MovementComp->SetMovementMode(MOVE_Walking);
	SetActorLocation(ClimbUpStandUpPosition);
	CheckOverlappingActors();
}



float ARYU2D_MainCharacterZD::GetMoveRightInput()
{
	return MoveRightInput;
}


//****DEBUGSECTION*******//


void ARYU2D_MainCharacterZD::DrawDebugInfosOnScreen()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("V(x): %s"), *currV.ToString()), false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("a(x): %s"), *currA.ToString()), false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Looking Right: %s"), bLookRight ? TEXT("true") : TEXT("false"), false));
	}

	FString MoveMode;
	FString ClimbMode;

	switch (PlayerMovement)
	{
	case EPlayerMovement::BEGINRUN:
		MoveMode = "BEGIN RUNNING";
		break;
	case EPlayerMovement::RUN:
		MoveMode = "RUNNING";
		break;
	case EPlayerMovement::ENDRUN:
		MoveMode = "END RUNNING";
		break;
	case EPlayerMovement::JUMPUP:
		MoveMode = "JUMPING";
		break;
	case EPlayerMovement::CANGRABLEDGE:
		MoveMode = "CanGrabLedge";
		break;
	case EPlayerMovement::CLIMBING:
		MoveMode = "Climbing";
		break;
	case EPlayerMovement::STARTTURN:
		MoveMode = "StartTurning";
	case EPlayerMovement::ENDTURN:
		MoveMode = "EndTurning";
		break;
	default:
		MoveMode = "STANDING";
	}

	switch (RYUClimbingMode)
	{
	case ERYUClimbingMode::NONE:
		ClimbMode = "Climbing inactive";
		break;
	case ERYUClimbingMode::HANGONLEDGE:
		ClimbMode = "HangingOnLedge";
		break;
	case ERYUClimbingMode::CLIMBDOWNLEDGE:
		ClimbMode = "ClimbingDownLedge";
		break;
	case ERYUClimbingMode::CLIMBUPLEDGE:
		ClimbMode = "ClimbingUpLedge";
		break;
	case ERYUClimbingMode::CANCLIMBDOWNLEDGE:
		ClimbMode = "CanClimbDownLedge";
		break;
	case ERYUClimbingMode::CANCLIMBUPLEDGE:
		ClimbMode = "CanClimbUpLedge";
		break;
	case ERYUClimbingMode::CANCLIMBUPANDDOWN:
		ClimbMode = "CanClimbUpAndDownLedge";
		break;
	case ERYUClimbingMode::FALLDOWNLEDGE:
		ClimbMode = "FallingDownLedge";
		break;
	case ERYUClimbingMode::CLIMBLADDERUP:
		ClimbMode = "ClimbingUpLadder";
		break;
	case ERYUClimbingMode::CLIMBLADDERDOWN:
		ClimbMode = "ClimbingDownLadder";
		break;
	default:
		break;
	}

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Movement: %s"), *MoveMode), false);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("ClimbingMode: %s"), *ClimbMode), false);
}

void ARYU2D_MainCharacterZD::DebugSomething()
{

}

void ARYU2D_MainCharacterZD::ChangeMovementMode()
{
	switch (PlayerMovement)
	{
	case EPlayerMovement::STAND:
		PlayerMovement = EPlayerMovement::CANGRABLEDGE;
		break;
	case EPlayerMovement::CANGRABLEDGE:
		PlayerMovement = EPlayerMovement::JUMPUP;
		break;
	case EPlayerMovement::JUMPUP:
		PlayerMovement = EPlayerMovement::CLIMBING;
		break;
	case EPlayerMovement::CLIMBING:
		PlayerMovement = EPlayerMovement::STAND;
		break;
	case EPlayerMovement::SNEAK:
		break;
	case EPlayerMovement::STANDUP:
		break;
	case EPlayerMovement::COMBAT:
		break;
	default:
		break;
	}
}

void ARYU2D_MainCharacterZD::ChangeClimbingMode()
{
	switch (RYUClimbingMode)
	{
	case ERYUClimbingMode::NONE:
		RYUClimbingMode = ERYUClimbingMode::CANCLIMBUPLEDGE;
		break;
	case ERYUClimbingMode::CANCLIMBUPLEDGE:
		RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
		break;
	case ERYUClimbingMode::CANCLIMBUPANDDOWN:
		RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
		break;
	case ERYUClimbingMode::CANCLIMBDOWNLEDGE:
		RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
		break;
	case ERYUClimbingMode::HANGONLEDGE:
		RYUClimbingMode = ERYUClimbingMode::CLIMBUPLEDGE;
		break;
	case ERYUClimbingMode::CLIMBUPLEDGE:
		RYUClimbingMode = ERYUClimbingMode::NONE;
		break;
	default:
		break;
	}
}
