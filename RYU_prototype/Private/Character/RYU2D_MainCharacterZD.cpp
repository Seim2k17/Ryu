// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_MainCharacterZD.h"
#include "RYU2D_MovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/RYU2D_MovementComponent.h"



ARYU2D_MainCharacterZD::ARYU2D_MainCharacterZD(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URYU2D_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bDebugOutputActive = true;

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
	onTimelineCallback.BindUFunction(this, FName("TimelineCallback"));
	onTimelineFinishedCallback.BindUFunction(this, FName("TimelineFinishedCallback"));
}


void ARYU2D_MainCharacterZD::UpdateCharacter()
{
	currA = GetCharacterMovement()->GetCurrentAcceleration();
	currV = this->GetVelocity();

	/*
	if ((!bStartedNoLoopAnimation) &&
		(PlayerMovement != EPlayerMovement::STARTTURN) &&
		(PlayerMovement != EPlayerMovement::BEGINRUN) &&
		(PlayerMovement != EPlayerMovement::ENDRUN))
	{
		float TravelDirection = currV.X;
		// Set the rotation so that the character faces his direction of travel.
		if (Controller != nullptr)
		{
			if (TravelDirection < 0.0f)
			{
				Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
				CameraBoom->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
			}
			else if (TravelDirection > 0.0f)
			{
				Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
				CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
			}
		}
	}
	*/
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

}

void ARYU2D_MainCharacterZD::StopJumping()
{

}


void ARYU2D_MainCharacterZD::DrawDebugInfosOnScreen()
{

}

void ARYU2D_MainCharacterZD::DebugSomething()
{

}

void ARYU2D_MainCharacterZD::Climb(float Val)
{

}


void ARYU2D_MainCharacterZD::MoveRight(float Val)
{
	/*
	if (Val > 0)
	{
		bLookRight = true;
	}
	else if (Val < 0)
	{
		bLookRight = false;
	}
	*/

	if ((PlayerMovement!= EPlayerMovement::BEGINRUN) &&
		(PlayerMovement != EPlayerMovement::ENDRUN) &&
		(PlayerMovement != EPlayerMovement::STARTTURN) &&
		(PlayerMovement != EPlayerMovement::TURN))
		{
			AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val);
		}
	
}

void ARYU2D_MainCharacterZD::MoveUp(float Value)
{

}

void ARYU2D_MainCharacterZD::HandleSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void ARYU2D_MainCharacterZD::HandleSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ARYU2D_MainCharacterZD::TimelineCallback(float val)
{

}

void ARYU2D_MainCharacterZD::TimelineFinishedCallback()
{

}

void ARYU2D_MainCharacterZD::PlayTimeline()
{

}

void ARYU2D_MainCharacterZD::SetCurrentTimelineParams(UCurveFloat* FloatCurve, bool TimelineIsLooping, bool IgnoreTimeDilation)
{

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
	PlayerMovement = EPlayerMovement::STAND;

	float TravelDirection = currV.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
			CameraBoom->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
			CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
		}
	}

	bLookRight = !bLookRight;
}
