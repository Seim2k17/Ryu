// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_CharacterPrince.h"
#include "PaperFlipbookComponent.h"
#include "RYU2D_MovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ClimbAssetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/RYU2D_AnimationComponent.h"
#include "RYU2D_AnimationComponent.h"
#include "Components/RYU2D_MovementComponent.h"




ARYU2D_CharacterPrince::ARYU2D_CharacterPrince(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URYU2D_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bDebugOutputActive = true;

	CreateDefaultSubobject<UClimbAssetComponent>(TEXT("ClimbAssetComponent"));

	Animation2DComponent = CreateDefaultSubobject<URYU2D_AnimationComponent>(TEXT("P2D_Animations"));

	InitializeCharacterValues();
}

void ARYU2D_CharacterPrince::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//TODO: take closer look when CustomMovement will be impl.
	MovementComp = Cast<URYU2D_MovementComponent>(GetCharacterMovement());
}

void ARYU2D_CharacterPrince::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

}



void ARYU2D_CharacterPrince::InitializeCharacterValues()
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

	/*

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

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


void ARYU2D_CharacterPrince::BeginPlay()
{
	Super::BeginPlay();

	GetSprite()->OnFinishedPlaying.AddDynamic(this, &ARYU2D_CharacterPrince::FlipbookFinishedPlaying);

	SphereTracer->OnComponentBeginOverlap.AddDynamic(this, &ARYU2D_CharacterPrince::HandleSphereColliderBeginOverlap);
	SphereTracer->OnComponentEndOverlap.AddDynamic(this, &ARYU2D_CharacterPrince::HandleSphereColliderEndOverlap);
}


void ARYU2D_CharacterPrince::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Temp save for Timelinestuff
	fDeltaSeconds = DeltaTime;

	UpdateCharacter();

	if (bDebugOutputActive)
	{
		DrawDebugInfosOnScreen();
	}
}

void ARYU2D_CharacterPrince::UpdateCharacter()
{
	// Update animation to match the motion

	UpdateAnimation();

	if ((!bStartedNoLoopAnimation) &&
		(PlayerMovement != EPlayerMovement::STARTTURN) &&
		(PlayerMovement != EPlayerMovement::BEGINRUN) &&
		(PlayerMovement != EPlayerMovement::ENDRUN))
	{
		// Now setup the rotation of the controller based on the direction we are travelling
		const FVector PlayerVelocity = GetVelocity();
		float TravelDirection = PlayerVelocity.X;
		// Set the rotation so that the character faces his direction of travel.
		if (Controller != nullptr)
		{
			if (TravelDirection < 0.0f)
			{
				Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
			}
			else if (TravelDirection > 0.0f)
			{
				Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
			}
		}
	}

	if (PlayerMovement == EPlayerMovement::CLIMBING)
	{
		/*TODO */
		if (CurrentTimeline != NULL)
		{
			CurrentTimeline->TickComponent(fDeltaSeconds, ELevelTick::LEVELTICK_TimeOnly, NULL);
		}
	
	}
}

//////////////////////////////////////////////////////////////////////////
// Animation
//////////////////////////////////////////////////////////////////////////
void ARYU2D_CharacterPrince::UpdateAnimation()
{
	// Are we moving or standing still?
	//for loopingAnimations

	/*
	if (this->GetVelocity().Z < 0)
	{
		CharAnimation2DState = ERYU2DAnimationState::FALLINGLOOP;
	}
	*/

	switch (CharAnimation2DState)
	{
	case  ERYU2DAnimationState::IDLE:
		PlayFlipBookAnimation(Animation2DComponent->IdleAnimation, true);
		return;
	case  ERYU2DAnimationState::RUNNING:
		PlayFlipBookAnimation(Animation2DComponent->RunningAnimation, true);
		return;
	case  ERYU2DAnimationState::STARTTURN:
		if (!bStartedNoLoopAnimation && Animation2DComponent->TurnAnimation)
		{
			if (!bLookRight)
			{
				//rotate first if not looking to the right
				Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
			}

			PlayFlipBookAnimation(Animation2DComponent->TurnAnimation, false);
			return;
		}
		return;

	case ERYU2DAnimationState::BEGINRUN:
		if (!bStartedNoLoopAnimation && Animation2DComponent->StartRunAnimation)
		{
			PlayFlipBookAnimation(Animation2DComponent->StartRunAnimation, false);
			return;
		}
		return;
	case ERYU2DAnimationState::ENDRUN:
		if (!bStartedNoLoopAnimation && Animation2DComponent->EndRunAnimation)
		{
			PlayFlipBookAnimation(Animation2DComponent->EndRunAnimation, false);
			return;
		}
		return;
	case ERYU2DAnimationState::JUMPUP:
		
		if (!bStartedNoLoopAnimation && Animation2DComponent->JumpUpAndFallAnimation)
		{
			PlayFlipBookAnimation(Animation2DComponent->JumpUpAndFallAnimation, false);
			return;
		}
		return;
	case ERYU2DAnimationState::FALLINGIN:
		if (!bStartedNoLoopAnimation && Animation2DComponent->FallingFromLedgeAnimation)
		{
			PlayFlipBookAnimation(Animation2DComponent->FallingFromLedgeAnimation, false);
			return;
		}
		return;
	case ERYU2DAnimationState::FALLINGLOOP:
		if (Animation2DComponent->FallingLoopAnimation)
		{
			PlayFlipBookAnimation(Animation2DComponent->FallingLoopAnimation, true);
			return;
		}
		return;
		//TODO
	case ERYU2DAnimationState::FALLINGOUT:
		if (!bStartedNoLoopAnimation && true)
		{
			PlayFlipBookAnimation(nullptr, false);
			return;
		}
		return;

	default:
		//assume animation is looping
		PlayFlipBookAnimation(nullptr, true);
	}
}



void ARYU2D_CharacterPrince::PlayFlipBookAnimation(UPaperFlipbook* AnimationToPlay, bool bAnimationIsLooping)
{
	if (AnimationToPlay != nullptr)
	{
		bStartedNoLoopAnimation = !bAnimationIsLooping;
		GetSprite()->SetLooping(bAnimationIsLooping);
		GetSprite()->SetFlipbook(AnimationToPlay);
		//REALLY IMPORTANT to reset Flipbbok when its not looping (in turn)!
		GetSprite()->PlayFromStart();
		//PlayerMovement = EPlayerMovement::TURN;
	}


	if (bAnimationIsLooping)
	{

		UPaperFlipbook* LoopAnimation;

		GetSprite()->SetLooping(true);
		//const FVector PlayerVelocity = GetVelocity();
		//const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

		//LoopAnimation = (PlayerSpeedSqr > 0.0f) ? Animation2DComponent->RunningAnimation : Animation2DComponent->IdleAnimation;
		LoopAnimation = AnimationToPlay;
				
		if (GetSprite()->GetFlipbook() != LoopAnimation)
		{
			GetSprite()->SetFlipbook(LoopAnimation);
			GetSprite()->PlayFromStart();
		}
	}
}



void ARYU2D_CharacterPrince::FlipbookFinishedPlaying()
{
	UE_LOG(LogTemp, Log, TEXT("FlipbookFinishedPlaying(): FlipBookAni Ended in %d"), GetSprite()->GetFlipbookLengthInFrames());
	switch (CharAnimation2DState)
	{
	case ERYU2DAnimationState::BEGINRUN:
		BeginRunFlipbookFinished();
		break;
	case ERYU2DAnimationState::ENDRUN:
		EndRunFlipbookFinished();
		break;
	case ERYU2DAnimationState::STARTTURN:
		StartTurnFlipbookFinished();
		break;
	case ERYU2DAnimationState::CLIMBING:
		ClimbingFlipbookFinished();
		break;
	
	}

}


void ARYU2D_CharacterPrince::BeginRunFlipbookFinished()
{
	PlayerMovement = EPlayerMovement::RUN;
	CharAnimation2DState = ERYU2DAnimationState::RUNNING;
	bStartedNoLoopAnimation = false;
	GetSprite()->SetLooping(true);
}


void ARYU2D_CharacterPrince::EndRunFlipbookFinished()
{
	PlayerMovement = EPlayerMovement::STAND;
	CharAnimation2DState = ERYU2DAnimationState::IDLE;
	bStartedNoLoopAnimation = false;
	GetSprite()->SetLooping(true);
}


void ARYU2D_CharacterPrince::StartTurnFlipbookFinished()
{
	UE_LOG(LogTemp, Log, TEXT("StartTurnFlipbookFinished(): TurnAni Ended"));
	// Now setup the rotation of the controller based on the direction we are travelling
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (bLookRight)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}

	bLookRight = !bLookRight;

	FVector CurPos = GetActorLocation();
	FVector NewPos = CurPos;

	//after Turn
	if (bLookRight)
	{
		NewPos.X = NewPos.X + TurnPositionOffset.X;
		NewPos.Z = NewPos.Z + TurnPositionOffset.Z;
	}
	else
	{
		NewPos.X = NewPos.X - TurnPositionOffset.X;
		NewPos.Z = NewPos.Z - TurnPositionOffset.Z;
	}

	SetActorLocation(NewPos);

	bStartedNoLoopAnimation = false;
	PlayerMovement = EPlayerMovement::STAND;
	CharAnimation2DState = ERYU2DAnimationState::IDLE;

	GetSprite()->SetLooping(true);
}

void ARYU2D_CharacterPrince::ClimbingFlipbookFinished()
{
	PlayerMovement = EPlayerMovement::STAND;
	CharAnimation2DState = ERYU2DAnimationState::IDLE;
	bStartedNoLoopAnimation = false;
	GetSprite()->SetLooping(true);
}


void ARYU2D_CharacterPrince::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARYU2D_CharacterPrince::MoveRight);
	PlayerInputComponent->BindAxis("Climb", this, &ARYU2D_CharacterPrince::MoveUp);
}

void ARYU2D_CharacterPrince::MoveRight(float Val)
{
	/*UpdateChar();*/
	// Apply the input to the character motion
	if (Val != 0)
	{
		//UE_LOG(LogTemp, Log, TEXT("MoveRight(): MOVE %s ; bLookRight: %s ; bPlayTurnAni: %s ; Sprite(looping) %s"), *FString::SanitizeFloat(Val),
		//	bLookRight ? TEXT("true") : TEXT("false"), bPlayTurnAni ? TEXT("true") : TEXT("false"), GetSprite()->IsLooping() ? TEXT("true") : TEXT("false"));

	}

	switch (PlayerMovement)
	{
		//TODO: easeIn
	case EPlayerMovement::BEGINRUN:
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val * 0.5f);
		break;
		//TODO: easeOut
	case EPlayerMovement::ENDRUN:
		float MoveValue = bLookRight ? 0.06f : -0.06f;
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveValue);
		break;
	}

	//needs a deeper look
	if ((PlayerMovement != EPlayerMovement::STARTTURN) &&
		(!bStartedNoLoopAnimation))
	{
		if ((bLookRight && Val > 0) || (!bLookRight && Val < 0))
		{
			AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val);

			switch (PlayerMovement)
			{
			case EPlayerMovement::STAND:
				PlayerMovement = EPlayerMovement::BEGINRUN;
				CharAnimation2DState = ERYU2DAnimationState::BEGINRUN;
				break;
			case EPlayerMovement::JUMPUP:
				//TODO
				break;
			case EPlayerMovement::FALLING:
				//TODO
				break;
			case EPlayerMovement::CLIMBING:
				//TODO
				break;
			case EPlayerMovement::SNEAK:
				//TODO
				break;
			default:
				break;
			}
		}
		else
		{
			if (Val != 0)
			{
				PlayerMovement = EPlayerMovement::STARTTURN;
				CharAnimation2DState = ERYU2DAnimationState::STARTTURN;
			}
			else
			{
				//just released key (needs another deeper look with Controller because COntrollerX = Axis
				if (PlayerMovement == EPlayerMovement::RUN)
				{
					PlayerMovement = EPlayerMovement::ENDRUN;
					CharAnimation2DState = ERYU2DAnimationState::ENDRUN;
				}
				else
				{
					if (!bStartedNoLoopAnimation)
					{
						PlayerMovement = EPlayerMovement::STAND;
						CharAnimation2DState = ERYU2DAnimationState::IDLE;
					}
				}

			}

		}
	}
}

void ARYU2D_CharacterPrince::MoveUp(float Value)
{
	
	if (Value != 0)
	{
		UE_LOG(LogTemp, Log, TEXT("MoveUp(): Climb up Prince is gone"));
		switch (PlayerMovement)
		{
			//TODO: check for all special cases !
		case EPlayerMovement::STAND:
			RYUClimbingMode = ERYUClimbingMode::NONE;
			
			//JumpUpTimeline
			if (Value > 0)
			{
				UE_LOG(LogTemp, Log, TEXT("MoveUp(): Climb up with Timeline"));
				if (Animation2DComponent->ClimbUpFloatCurve)
				{
					SetCurrentTimelineParams(Animation2DComponent->JumpUpFloatCurve, false, true);
				}
			}
			else
			{
				//Crouch
			}
			Climb(Value);
			break;

			//...near a ledge needable ??? need to check if this makes fun: soo you need to click 2times the button to climb up ? maybe if there is danger ?
		case EPlayerMovement::CANGRABLEDGE:
		{
			if (RYUClimbingMode == ERYUClimbingMode::NONE)
			{
				//UpOrDown the ledge ?
				CanGrabLedges(Value);
				break;
			}
		}
		//TODO important: LADDER UpANdDown
		default:
			break;
		}
	}
}

void ARYU2D_CharacterPrince::Climb(float Val)
{
	switch (RYUClimbingMode)
	{

		case ERYUClimbingMode::NONE:
			UE_LOG(LogTemp, Log, TEXT("Climb(): JUMPUpOrDown"));
			JumpUpOrDown(Val, GetActorLocation());
			break;
		case ERYUClimbingMode::CANCLIMBUPLEDGE:
			UE_LOG(LogTemp, Log, TEXT("Climb(): CanClimbUp"));
			CanClimbUp(Val, GetActorLocation());
			break;
		case ERYUClimbingMode::CANCLIMBDOWNLEDGE:
			CanClimbDown(Val);
			break;
		case ERYUClimbingMode::CANCLIMBUPANDDOWN:
			CanClimbUpAndDown(Val, GetActorLocation());
			break;
		case ERYUClimbingMode::CLIMBDOWNLEDGE:
			break;
		case ERYUClimbingMode::CLIMBUPLEDGE:
			break;
		case ERYUClimbingMode::FALLDOWNLEDGE:
			break;
		case ERYUClimbingMode::HANGONLEDGE:
			HangOnLedgeAndClimb(Val);
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



void ARYU2D_CharacterPrince::JumpUpOrDown(float Val, FVector StartJumpPosition)
{
	//TODO RESORT THINGS !!!
	if (Val > 0)
	{
		PlayerMovement = EPlayerMovement::JUMPUP;
		CharAnimation2DState = ERYU2DAnimationState::JUMPUP;
		//TODO: only set ClimbingMode when there is a ledge to climb!
		

		//** Initialize the Start End Endpoints 
		Animation2DComponent->ClimbUpStartTimelineLocation = GetActorLocation();
		Animation2DComponent->ClimbUpEndTimelineLocation = FVector(Animation2DComponent->ClimbUpStartTimelineLocation.X, Animation2DComponent->ClimbUpStartTimelineLocation.Y,
			Animation2DComponent->ClimbUpStartTimelineLocation.Z + Animation2DComponent->ClimbUpOffset);

		//GetMovementComponent()->SetMovementMode(MOVE_Flying);

		PlayTimeline();

	}
	else
	{
		//TODO: Character climbs down OR get into Crouchmode !
		PlayerMovement = EPlayerMovement::CLIMBING;
	}
}


void ARYU2D_CharacterPrince::CanClimbUp(float Val, FVector StartClimbUpPosition)
{
	if (Val > 0.8)
	{
		if (GetLedgeHangPosition() != FVector::ZeroVector)
		{
			UE_LOG(LogTemp, Warning, TEXT("NotZeroButSet ??"));
			if (!bHangPositionSet)
			{

				_StartClimbUpPosition = StartClimbUpPosition;
				UE_LOG(LogTemp, Warning, TEXT("StartHangLedgePosition: %s"), *_StartClimbUpPosition.ToString());
				
				bHangPositionSet = true;

				PlayerMovement = EPlayerMovement::CLIMBING;
				//TODO later climbUpToHangposition Ani
				RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
				//CustMovementComp->SetMovementMode(MOVE_Flying);
				//GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor::Respawn, CoolDownDuration);

				FVector _HangLocation = GetLedgeHangPosition();


				ERYULedgeSideEntered _SideEntered = GetLedgeSideEntered();

				switch (_SideEntered)
				{
				case ERYULedgeSideEntered::NONE:
					break;
				case ERYULedgeSideEntered::LeftSide:
					//SetActorRotation(FRotator(0, -90.0f, 0));
					break;
				case ERYULedgeSideEntered::RightSide:
					//SetActorRotation(FRotator(0, 90.0f, 0));
					break;
				default:
					break;
				}

				
				MovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::HANGONLEDGE));

				SetActorLocation(_HangLocation);
				UE_LOG(LogTemp, Warning, TEXT("HangLedgePosition : %s"), *_HangLocation.ToString());
				//TODO: later replace with JumpUpHangAni
				//PlayAnimMontage(ClimbAssetComp->ClimbHangMontage, 1.0f);

				

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HangLedgePosition set to (0,0,0)"))
		}

	}
}


void ARYU2D_CharacterPrince::CanClimbDown(float Val)
{
	
}


void ARYU2D_CharacterPrince::CanClimbUpAndDown(float Val, FVector param2)
{
	
}


void ARYU2D_CharacterPrince::HangOnLedgeAndClimb(float Val)
{
	
}


void ARYU2D_CharacterPrince::CanGrabLedges(float Val)
{
	
}



void ARYU2D_CharacterPrince::Jump()
{

}

void ARYU2D_CharacterPrince::StopJumping()
{

}


void ARYU2D_CharacterPrince::HandleSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void ARYU2D_CharacterPrince::HandleSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


/** Start Timeline specific Stuff*/

void ARYU2D_CharacterPrince::TimelineCallback(float val)
{
	// This function is called for every tick in the timeline.
	//UE_LOG(LogTemp, Log, TEXT("Here incr. z-Location: %s"), *FString::SanitizeFloat(val));

	FVector StartTLLocation;
	FVector EndTLLocation;

	switch (PlayerMovement)
	{
	case EPlayerMovement::CLIMBING:
		//TODO: look for ClimbUp vs. Down etc // see mix from 3D LedgeClimbing but wo Rootmotion ...
		StartTLLocation = Animation2DComponent->ClimbUpStartTimelineLocation;
		EndTLLocation = Animation2DComponent->ClimbUpEndTimelineLocation;
		break;
	default:
		break;
	}
	
	SetActorLocation(FMath::Lerp(StartTLLocation, EndTLLocation, val));
	//AddActorLocalOffset(FVector(0,0,CurrentTimeline->));
}


void ARYU2D_CharacterPrince::TimelineFinishedCallback()
{
	// This function is called when the timeline finishes playing.
	switch (PlayerMovement)
	{
	case EPlayerMovement::CLIMBING:
		SetActorLocation(Animation2DComponent->ClimbUpEndTimelineLocation);
		break;
	default:
		break;
	}
	
}


void ARYU2D_CharacterPrince::PlayTimeline()
{
	if (CurrentTimeline != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("Play a timeline from Start"));
		CurrentTimeline->PlayFromStart();
	}

}


void ARYU2D_CharacterPrince::SetCurrentTimelineParams(UCurveFloat* FloatCurve, bool TimelineIsLooping, bool IgnoreTimeDilation)
{
	if (FloatCurve)
	{
		UE_LOG(LogTemp, Log, TEXT("Set TimeLine Params for Timeline %s"), *FloatCurve->GetName());
		//Add Float curve to the timeline and bind it to the interpfunction´s delegate
		//3rd Parameter = floatValue, Propertyname, Bind all Stuff
		CurrentTimeline->AddInterpFloat(FloatCurve, onTimelineCallback, FName("Movement"));
		CurrentTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		// Timeline Settings
		CurrentTimeline->SetLooping(TimelineIsLooping);
		//false = it moves as the global Time Dileation
		CurrentTimeline->SetIgnoreTimeDilation(IgnoreTimeDilation);
	}
}

/** End Timeline specific Stuff*/

/** Output Log on Screen */
void ARYU2D_CharacterPrince::DrawDebugInfosOnScreen()
{
	currA = GetCharacterMovement()->GetCurrentAcceleration();
	currV = this->GetVelocity();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("V(x): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.X))), false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("V(y): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Y))), false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("V(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Z))), false);

		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("a(x): %s"), *FString::SanitizeFloat(currA.X)), false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("a(y): %s"), *FString::SanitizeFloat(currA.Y)), false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("a(z): %s"), *FString::SanitizeFloat(currA.Z)), false);

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


		//UE_LOG(LogTemp, Log, TEXT("V(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Z)));

		if (bJumpJustStarted)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("V(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Z))), false);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("JumpZV(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(GetCharacterMovement()->JumpZVelocity))), false);
		}
	}
}

void ARYU2D_CharacterPrince::DebugSomething()
{

}
