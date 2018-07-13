// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUCharacterIchi.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/RYUCustomizeMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Character/RYUENUM_LedgePos.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/ClimbAssetComponent.h"


// Sets default values
ARYUCharacterIchi::ARYUCharacterIchi(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URYUCustomizeMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PlayerActive = ERYUPlayerActive::Player1;

	ClimbAssetComp = CreateDefaultSubobject<UClimbAssetComponent>(TEXT("ClimbAssetComponent"));

	InitializeCharacterValues();

}

void ARYUCharacterIchi::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CustMovementComp = Cast<URYUCustomizeMovementComponent>(GetCharacterMovement());
}

void ARYUCharacterIchi::InitializeCharacterValues()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

															  // Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 325.0f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	//starts slowly until reach full speed
	GetCharacterMovement()->MaxAcceleration = 850.f;
	//if character should stop, it needs some time, not instant
	GetCharacterMovement()->BrakingDecelerationWalking = 50.0f;
	//the Higher the friction the faster the character stops
	GetCharacterMovement()->GroundFriction = 2.0f;

	bDebugOutputActive = true;


	DefaultGravityScale = GetCharacterMovement()->GravityScale;
	MaxGravityScaleStd = 3.0;

	CharMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	StartJumpVelocity.X = 0;
	StartJumpVelocity.Y = 0;
	StartJumpVelocity.Z = GetCharacterMovement()->JumpZVelocity;

	tmpCheck = false;

	FallCheck = false;

	CoyoteJumpPossible = false;

	TreshholdYWalkRun = 0.3f;

	bJumpJustStarted = false;

	bAllowClimbUp = false;

	AllowClimbUpTime = 0.8f;

	bHangPositionSet = false;

	bDoOnceClimbInput = false;

}

// Called when the game starts or when spawned
void ARYUCharacterIchi::BeginPlay()
{
	Super::BeginPlay();
	//it works in BeginPlay but not in constructor : thats why in an existed Character - BP : AddDynamic gets not registered!
	//--> entweder you need to make a new Char - BP then AddDynamic works in contructor or better you use register the Overlap Method(Adddynamic) in BeginPlay
	SphereTracer->OnComponentBeginOverlap.AddDynamic(this, &ARYUCharacterBase::OnSphereTracerHandleBeginOverlap);
	SphereTracer->OnComponentEndOverlap.AddDynamic(this, &ARYUCharacterBase::OnSphereTracerHandleEndOverlap);

	if (CustMovementComp->GravityScaleMaximum == 0)
	{
		CustMovementComp->SetGravityScaleMaximum(MaxGravityScaleStd);
	}

	CustMovementComp->SetNormalMaxJumpCount(JumpMaxCount);

	

}

void ARYUCharacterIchi::Tick(float DeltaTime)
{
	//set Movement Enum in Parent Class RYUCharacterBase !
	Super::Tick(DeltaTime);

	currentFPS = 1 / DeltaTime;

	/** Check if a ledge is nearby to climb, or hang, or snap jumping to it Implemented in BaseClass: RYUCharacterBase*/
	/** Replace with simplier logic: Just Tag the Objects and play appropriate animation, FUCKING LedgeTrace doesn´t work like it should, maybe its because the game is pseudo 3D, mainly 2D*/
	//CheckLedgeTracer();

 	if (bDebugOutputActive)
	{
		DrawDebugInfosOnScreen();
	}
	
	//check preferences after Jump is pressed
	AfterJumpButtonPressed();
}

void ARYUCharacterIchi::DrawDebugInfosOnScreen()
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

		switch (RYUMovement)
		{
			case ERYUMovementMode::WALK:
				MoveMode = "WALKING";
				break;

			case ERYUMovementMode::RUN:
				MoveMode = "RUNNING";
				break;
			case ERYUMovementMode::JUMP:
				MoveMode = "JUMPING";
				break;
			case ERYUMovementMode::CANGRABLEDGE:
				MoveMode = "CanGrabLedge";
				break;
			case ERYUMovementMode::CANTRACELEDGE:
				MoveMode = "CanTraceLedge";
				break;
			case ERYUMovementMode::CLIMB:
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


void ARYUCharacterIchi::DebugSomething()
{
	//CustMovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUMovementMode::CLIMBLEDGE));
	ERYULedgePosition LedgePosi = ERYULedgePosition::Above_150cm;
	CustMovementComp->OnCanClimbLedge.Broadcast(LedgePosi);
}


void ARYUCharacterIchi::ReSetHangUpPosition()
{
	bHangPositionSet = false;
}

void ARYUCharacterIchi::Jump()
{

	if (RYUClimbingMode == ERYUClimbingMode::NONE)
	{
		switch (RYUMovement)
		{
		case ERYUMovementMode::CANGRABLEDGE:
		{
			//CanGrabLedgeAndClimb(1.0f);
			CanGrabLedges(1.0f);
			//break;
			return;
		}
		case ERYUClimbingMode::CANCLIMBUPLEDGE:
		{
			//break;
			OnCanClimbUp.Broadcast(1.0f);
			//CanClimbUp(1.0f, HangLedgePosition, GetActorLocation());
			return;
		}

		case ERYUClimbingMode::HANGONLEDGE:
		{
			//break;
			HangOnLedgeAndClimb(1.0f);
			//CanClimbUp(1.0f, HangLedgePosition, GetActorLocation());
			return;
		}

		}
	}
	

	switch (RYUClimbingMode)
	{
		case ERYUClimbingMode::NONE:
			break;
		case ERYUClimbingMode::CANCLIMBUPLEDGE:
			Climb(1.0f);
			return;
			//break;
		case ERYUClimbingMode::CANCLIMBDOWNLEDGE:
			break;
		case ERYUClimbingMode::CANCLIMBUPANDDOWN:
			break;
		case ERYUClimbingMode::CLIMBDOWNLEDGE:
			break;
		case ERYUClimbingMode::CLIMBUPLEDGE:
			break;
		case ERYUClimbingMode::FALLDOWNLEDGE:
			break;
		case ERYUClimbingMode::HANGONLEDGE:
			break;
		case ERYUClimbingMode::CLIMBLADDERUP:
			break;
		case ERYUClimbingMode::CLIMBLADDERDOWN:
			break;
		default:
			break;
	}

	if (bJumpJustStarted == false)
	{
		bJumpJustStarted = true;
		TimeDeltaStart = GetWorld()->GetTimeSeconds();
	}


	StartJumpPosition = GetActorLocation();

	//suppose wie´re not falling -> to make it a real jump not adding JumpZVelo to Vel(Z) < 0
	if (CoyoteJumpPossible)
	{
		GetCharacterMovement()->Velocity.Z = 0;
	}

	DeactivateCoyotoeJumpPossible();

	bPressedJump = true;
	
	float JumpKeyMaxTime = JumpMaxHoldTime * 1000;
	//UE_LOG(LogTemp, Log, TEXT("JumpKeyMaxTime ms: %s"), *FString::SanitizeFloat(JumpKeyMaxTime));

	float InputY = GetInputAxisValue("MoveRight");
	
	StartJumpVelocity.Y = GetCharacterMovement()->Velocity.Y;
	if (FMath::Abs(StartJumpVelocity.Y) < 30.0f)
	{
		GetCharacterMovement()->Velocity.Y = CustMovementComp->JumpForce.Y * InputY * (-1.0f);
	}
	else
	{
		//AddMoreVelocity ?
		GetCharacterMovement()->Velocity.Y = GetCharacterMovement()->Velocity.Y + CustMovementComp->JumpForceRun.Y;
	}

	if ((RYUMovement != ERYUMovementMode::CANGRABLEDGE) &&
		(RYUMovement != ERYUMovementMode::CANTRACELEDGE) &&
		(RYUClimbingMode != ERYUClimbingMode::HANGONLEDGE))
	{
		RYUMovement = ERYUMovementMode::JUMP;
	}
	//UE_LOG(LogTemp, Log, TEXT("Y(final): %s"), *FString::SanitizeFloat(StartJumpVelocity.Y));

	//look character Jump Sheet for customization when level up
}

void ARYUCharacterIchi::StopJumping()
{
	//UE_LOG(LogTemp, Log, TEXT("Char: %s Release Jumping button."), *GetName());
	float JumpKeyTime = JumpKeyHoldTime * 1000;
	//UE_LOG(LogTemp, Log, TEXT("JumpKeypressed ms: %s"), *FString::SanitizeFloat(JumpKeyTime));
	//UE_LOG(LogTemp, Log, TEXT("JumpKeyCount: %s"), *FString::FromInt(JumpCurrentCount));
	//Super::StopJumping();
	bPressedJump = false;

	Super::ResetJumpState();
}

//called in Tick
void ARYUCharacterIchi::AfterJumpButtonPressed()
{

	//we suppose Jumping occurred / or falling from a ledge
	if (GetCharacterMovement()->IsMovingOnGround() == false)
	{
		//@ToDo: recheck/pimp the coyoteshitjump
		//then add CustomizedMovementData also in this Component (?), then we can remove the MultipleJump Conditions and directly ask st like:
		//
		// 		if (jumpInput) {
		// 			if (onGround || graceTimer > 0) {
		// 				jump();
		// 			}
		// 		}
		// 
		// 		//somewhere else in the code:
		// 		public function jump() :void {
		// 			//pretend theres more jumping logic here
		// 			graceTimer = 0;
				
		
		// 		if ((GetWorldTimerManager().GetTimerRemaining(Timerhandle_CoyoteTime) > 0) && CustMovementComp->CoyoteTimeActive)
		// 		{
		// 			CoyoteJumpPossible = true;
		// 			JumpMaxCount++;
		// 			UE_LOG(LogTemp, Log, TEXT("JumpMaxCount: %s "), *FString::FromInt(JumpMaxCount));
		// 		}

		//Coyote Time, if character falls (but should only trigger when falluing from ledge)
		//--> move to "JumpInput"
		
		if (!GetWorldTimerManager().IsTimerActive(CustMovementComp->Timerhandle_CoyoteTime) && (FMath::FloorToInt(GetActorLocation().Z) < FMath::FloorToInt(StartJumpPosition.Z)) 
			&& !CoyoteJumpPossible)
		{
			CoyoteJumpPossible = true;
			JumpMaxCount = CustMovementComp->GetNormalMaxJumpCount() + 1;
			//UE_LOG(LogTemp, Log, TEXT("Coyote-Timer Started: %s ms at: :%s , Jump started at: %s"), *FString::SanitizeFloat(CustMovementComp->CoyoteTime),*GetActorLocation().ToString(), *StartJumpPosition.ToString());
			GetWorldTimerManager().SetTimer(CustMovementComp->Timerhandle_CoyoteTime, this, &ARYUCharacterIchi::DeactivateCoyotoeJumpPossible, CustMovementComp->CoyoteTime, false);
		}
		
	
		//falling Down; grappy est. difference more than 30
		if ((GetVelocity().Z - GetCharacterMovement()->JumpZVelocity) < -40)
		{
		
			if (tmpCheck == false)
			{
				TimeDeltaEnd = GetWorld()->GetTimeSeconds();
				//UE_LOG(LogTemp, Log, TEXT("Peak reached: %s after: %s "), *FString::FromInt(JumpCurrentCount), *FString::SanitizeFloat(TimeDeltaEnd - TimeDeltaStart));
				tmpCheck = true;

			}

			//fall straight down
			if (GetInputAxisValue("MoveRight") == 0)
			{
				GetCharacterMovement()->Velocity.Y = 0;
				//? GetCharacterMovement()->Velocity.Y = GetCharacterMovement()->Velocity.Y / 20.0f;
			}
			//increase gravity (fall down faster)
			if (GetCharacterMovement()->GravityScale < CustMovementComp->GravityScaleMaximum)
			{
				GetCharacterMovement()->GravityScale += CustMovementComp->AddFallingMultiplierNumber;
			}
			//UE_LOG(LogTemp, Log, TEXT("GravScale: %s"),*FString::SanitizeFloat(GetCharacterMovement()->GravityScale));
		}

		//when character falls over certain Heigth
		if (GetVelocity().Z <= CustMovementComp->AfterJumpTreshold.Z) FallCheck = true;
	}
	else
	{
		if (bJumpJustStarted)
		{
			bJumpJustStarted = false;

			UE_LOG(LogTemp, Log, TEXT("bJumpJustStarted == false"));
			float directionV = (GetCapsuleComponent()->GetForwardVector().Y > 0) ? 1.0f : -1.0f;
			//Wenn Char schnell gelaufen ist oder aus grosser Höhe gefallen, @ToDo BigHeigth
			if ((FMath::Abs(StartJumpVelocity.Y) > CustMovementComp->AfterJumpTreshold.Y) || FallCheck)
			{
				//Add little Velocity after hitting the ground
				GetCharacterMovement()->Velocity.Y = CustMovementComp->VelocityAfterJumping.Y * (directionV);
			}

			FallCheck = false;
			//if fall on the ground earlier than MaxJumpButtonHoldTime
			CoyoteJumpPossible = false;
			StopJumping();

			//only when we´re really in jump mode and not "accidently" falling into LedgeTrace/Grab mode
			if (RYUMovement == ERYUMovementMode::JUMP)
			{
				RYUMovement = ERYUMovementMode::STAND;
			}
			
					
			//GetCharacterMovement()->GravityScale = DefaultGravityScale;
		}
		return;
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void ARYUCharacterIchi::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	//Axis-Bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ARYUCharacterIchi::MoveRight);

	//Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARYUCharacterIchi::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARYUCharacterIchi::StopJumping);

	PlayerInputComponent->BindAction("ChangePlayer", IE_Pressed, this, &ARYUCharacterIchi::ChangePlayer);

	PlayerInputComponent->BindAxis("ClimbUp", this, &ARYUCharacterIchi::Climb);

	PlayerInputComponent->BindAction("DebugKey_1", IE_Pressed, this, &ARYUCharacterIchi::DebugSomething);

	
	//Predefined Bindings
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ARYUCharacterIchi::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ARYUCharacterIchi::TouchStopped);
}


void ARYUCharacterIchi::ChangePlayer()
{
	switch (PlayerActive)
	{
	case ERYUPlayerActive::Player1:
		PlayerActive = ERYUPlayerActive::Player2;
		break;
	case ERYUPlayerActive::Player2:
		PlayerActive = ERYUPlayerActive::Player1;
		break;
	default:
		break;
	}
	OnPlayerActive.Broadcast();
}


void ARYUCharacterIchi::DeactivateCoyotoeJumpPossible()
{
	JumpMaxCount = CustMovementComp->GetNormalMaxJumpCount();
	
	if (GetWorldTimerManager().IsTimerActive(CustMovementComp->Timerhandle_CoyoteTime))
	{
		GetWorldTimerManager().ClearTimer(CustMovementComp->Timerhandle_CoyoteTime);
		//UE_LOG(LogTemp, Log, TEXT("ClearCoyoteTimer"));
	}

	return;
	
}

void ARYUCharacterIchi::MoveRight(float Val)
{

	//@ToDo: Move Setting MovementCode in TICK !!!! else there occure bad things when overlapping with LedgeTraces etc. and inkonsequences

	// add movement in that direction
	
	if ((RYUMovement != ERYUMovementMode::CLIMB) && 
		(RYUMovement != ERYUMovementMode::JUMP) &&
		(RYUMovement != ERYUMovementMode::CANTRACELEDGE) &&
		(RYUMovement != ERYUMovementMode::CANGRABLEDGE))
	{
		if (FMath::Abs(Val) > 0)
		{
			if (FMath::Abs(Val) > TreshholdYWalkRun)
				{
					RYUMovement = ERYUMovementMode::RUN;
				}
				else
				{
					RYUMovement = ERYUMovementMode::WALK;
				}
		}
		else
		{
			RYUMovement = ERYUMovementMode::STAND;
		}
	}

	if (RYUMovement != ERYUMovementMode::CLIMB) 
	{
		AddMovementInput(FVector(0.f, -1.f, 0.f), Val);
	}

	

	
	
	
}


void ARYUCharacterIchi::Climb(float Val)
{
		//Move up or Down a ledge...
		switch (RYUMovement)
		{
				//...near a ledge
			case ERYUMovementMode::CANGRABLEDGE:
			{
				if (RYUClimbingMode ==  ERYUClimbingMode::NONE)
				{
					CanGrabLedges(Val);
					//CanGrabLedgeAndClimb(Val);
					break;
				}
			}
			default:
				break;
		}

		switch (RYUClimbingMode)
		{
			case ERYUClimbingMode::NONE:
				break;
			case ERYUClimbingMode::CANCLIMBUPLEDGE:
				CanClimbUp(Val,GetActorLocation());
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
			case ERYUClimbingMode::CLIMBLADDERUP:
				break;
			case ERYUClimbingMode::CLIMBLADDERDOWN:
				break;
			default:
				break;
		}
}


void ARYUCharacterIchi::CanClimbUp(float Val, FVector StartClimbUpPosition)
{
// 	if (!GetMovementComponent()->IsFalling())
// 	{
	if (Val > 0.8)
	{
		if (GetLedgeHangPosition() != FVector::ZeroVector)
		{
			UE_LOG(LogTemp, Warning, TEXT("NotZeroButSet ??"));
			if (!bHangPositionSet)
			{

				_StartClimbUpPosition = StartClimbUpPosition;
				UE_LOG(LogTemp, Warning, TEXT("StartHangLedgePosition: %s"), *_StartClimbUpPosition.ToString());
				//HangLedgePosition = HangLedgePosition;
				//LedgeHangPosition.Z = LedgeHangPosition.Z + 115.0f;


				bHangPositionSet = true;

				RYUMovement = ERYUMovementMode::CLIMB;
				//@ToDo later climbUpToHangposition Ani
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
					SetActorRotation(FRotator(0, -90.0f, 0));
					break;
				case ERYULedgeSideEntered::RightSide:
					SetActorRotation(FRotator(0, 90.0f, 0));
					break;
				default:
					break;
				}
				

				CustMovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::HANGONLEDGE));

				SetActorLocation(_HangLocation);
				UE_LOG(LogTemp, Warning, TEXT("HangLedgePosition : %s"), *_HangLocation.ToString());
				//@ToDo: later replace with JumpUpHangAni
				PlayAnimMontage(ClimbAssetComp->ClimbHangMontage, 1.0f);

				GetWorldTimerManager().SetTimer(AllowClimbUp_TimerHandle, this, &ARYUCharacterIchi::SetAllowClimbUpTrue, AllowClimbUpTime, false, AllowClimbUpTime);

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HangLedgePosition set to (0,0,0)"))
		}

	}
//	}

}

void ARYUCharacterIchi::CanClimbDown(float Val)
{
	if (!GetMovementComponent()->IsFalling())
	{
		//Stand near ledge & Climb down to Hangmode
		if (Val < -0.5)
		{
			if (GetLedgeHangPosition() != FVector::ZeroVector)
			{
				if (!bHangPositionSet)
				{
					RYUMovement = ERYUMovementMode::CLIMB;
					RYUClimbingMode = ERYUClimbingMode::CLIMBDOWNLEDGE;

					FVector _HangLocation = GetLedgeHangPosition();
					ERYULedgeSideEntered _SideEntered = GetLedgeSideEntered();

					switch (_SideEntered)
					{
					case ERYULedgeSideEntered::NONE:
						break;
					case ERYULedgeSideEntered::LeftSide:
						SetActorRotation(FRotator(0, 90.0f, 0));
						break;
					case ERYULedgeSideEntered::RightSide:
						SetActorRotation(FRotator(0, -90.0f, 0));
						break;
					default:
						break;
					}
					SetActorLocation(_HangLocation);



					//@ToDo: Play MontageSection ClimbDown in Reverse
					if (ClimbAssetComp->ClimbDownMontage)
					{
						UE_LOG(LogTemp, Log, TEXT("StartPlaying: %s"), *ClimbAssetComp->ClimbDownMontage->GetName());
					}
					CustMovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::CLIMBDOWNLEDGE));
					//CustMovementComp->SetMovementMode(MOVE_Flying);
					PlayAnimMontage(ClimbAssetComp->ClimbDownMontage, 1.0f);

					bHangPositionSet = true;
					//ReSetHangUpPosition();
				}
			}
		}
	}

}

void ARYUCharacterIchi::CanClimbUpAndDown(float Val, FVector StartClimbPosition)
{
	//we have input
	if (Val != 0)
	{
		//crappy code ? !
		if (!bDoOnceClimbInput)
		{
			UE_LOG(LogTemp, Log, TEXT("Climb(z): %s"), *FString::SanitizeFloat(Val));
			ERYUClimbUpOrDownMode ClimbUpOrDown = ERYUClimbUpOrDownMode::NONE;

			if (Val > 0.8)
			{	
				bDoOnceClimbInput = true;
				ClimbUpOrDown = ERYUClimbUpOrDownMode::CLIMBUP;
			}
			else
			{
				if (Val < -0.5)
				{
					bDoOnceClimbInput = true;
					ClimbUpOrDown = ERYUClimbUpOrDownMode::CLIMDOWN;
				}
			}

			if (ClimbUpOrDown != ERYUClimbUpOrDownMode::NONE)
			{
				for (int i = 0; i < CapsuleOverlappedComponents.Num(); i++)
				{
					UBoxComponent* BoxTrigger = Cast<UBoxComponent>(CapsuleOverlappedComponents[i]);
					//BLUB save the Overlapped Components not the actors and check tags (die ich ja schon hab)
					if (BoxTrigger)
					{

						ARYUClimbingActor* ClimbAct = Cast<ARYUClimbingActor>(BoxTrigger->GetOwner());
						if (ClimbAct)
						{
							if ((ClimbUpOrDown == ERYUClimbUpOrDownMode::CLIMBUP) &&
								(BoxTrigger->ComponentTags[0] == CanClimbUpTagName) &&
								(BoxTrigger->ComponentTags[1] == LeftPositionTagName))
							{
								//Get ClimbUpPosition Left
								SetLedgeHangPosition(ClimbAct->LeftHangPosition->GetComponentLocation(), LeftPositionTagName);
							}

							if ((ClimbUpOrDown == ERYUClimbUpOrDownMode::CLIMBUP) &&
								(BoxTrigger->ComponentTags[0] == CanClimbUpTagName) &&
								(BoxTrigger->ComponentTags[1] == RightPositionTagName))
							{
								//Get ClimbUpPosition Right
								SetLedgeHangPosition(ClimbAct->RightHangPosition->GetComponentLocation(), RightPositionTagName);
							}

							if ((ClimbUpOrDown == ERYUClimbUpOrDownMode::CLIMDOWN) &&
								(BoxTrigger->ComponentTags[0] == CanClimbDownTagName) &&
								(BoxTrigger->ComponentTags[1] == LeftPositionTagName))
							{
								//Get ClimbDownPosition Left
								SetLedgeHangPosition(ClimbAct->LeftHangPosition->GetComponentLocation(), LeftPositionTagName);
							}

							if ((ClimbUpOrDown == ERYUClimbUpOrDownMode::CLIMDOWN) &&
								(BoxTrigger->ComponentTags[0] == CanClimbDownTagName) &&
								(BoxTrigger->ComponentTags[1] == RightPositionTagName))
							{
								//Get ClimbDownPosition Right
								SetLedgeHangPosition(ClimbAct->RightHangPosition->GetComponentLocation(), RightPositionTagName);
							}
						}

					}
				}
			
			}


			if (ClimbUpOrDown == ERYUClimbUpOrDownMode::CLIMBUP)
			{
				CanClimbUp(Val, GetActorLocation());

			}

			if (ClimbUpOrDown == ERYUClimbUpOrDownMode::CLIMDOWN)
			{
				CanClimbDown(Val);
			}
		}
	}
	
}

void ARYUCharacterIchi::CheckClimbingLedge()
{
	//@ToDo: resp. which ledge is it: Wall (= incl.Height) or Height (without a wall infront of the char) -> use appr. Animation for climbing
	//UE_LOG(LogTemp, Log, TEXT("Can Climb ledge from derived Class"));
	//here we can set/make stuff when we are in Position to climb a ledge (RYUMovement = ERYUMovementMode::CANGRABLEDGE), called in Super::Tick()
}


void ARYUCharacterIchi::CanGrabLedgeAndClimb(float Val)
{
	//Stand near ledge & climb Up or over ledge
	if (Val > 0.8)
	{
		ERYULedgePosition LedgePosi;
		if (!CustMovementComp->IsFalling())
		{
			CustMovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::CLIMBUPLEDGE));
			//CustMovementComp->SetMovementMode(MOVE_Flying);
			//select appr. Asset from height of ledge
			UE_LOG(LogTemp, Log, TEXT("LedgeHeightVector: %s"), *LedgeTracerHeight.ToString());

			TArray<FName> AcTags;

			//justInitReason
			LedgePosi = ERYULedgePosition::NONE;

			if (SphereOverlappedActor)
			{
				UE_LOG(LogTemp, Log, TEXT("There´s a Overlapping Actor"));
				AcTags = SphereOverlappedActor->Tags;
			}


			//YOU IDIOT !

			if (FMath::IsWithinInclusive(LedgeTracerHeight.Z, 0.0f, 110.0f))
			{
				LedgePosi = ERYULedgePosition::Above_080cm;
			}

			if (FMath::IsWithinInclusive(LedgeTracerHeight.Z, 110.01f, 130.0f))
			{
				LedgePosi = ERYULedgePosition::Above_100cm;
			}

			if (FMath::IsWithinInclusive(LedgeTracerHeight.Z, 130.01f, 180.0f))
			{
				LedgePosi = ERYULedgePosition::Above_150cm;
			}

			//possibly a workaround for climbing !?

			if (AcTags.Num() > 0)
			{
				UE_LOG(LogTemp, Log, TEXT("There´re Tags"));
				//Override the Ledgeposition set before, when climbing tagged (Ledgetracer is ignored
				if (AcTags[0] == "climbing") LedgePosi = ERYULedgePosition::Above_450cm;
				if (AcTags[0] == "hurdle") LedgePosi = ERYULedgePosition::Hurdle_080cm;

			}
			CustMovementComp->OnCanClimbLedge.Broadcast(LedgePosi);
		}
	}
}


void ARYUCharacterIchi::CanGrabLedges(float Val)
{
	if (SphereOverlappedActor != nullptr)
	{
		//we can only have one Tag resp. the first Tag is used
		TArray<FName> OverlapTags = SphereOverlappedActor->Tags;
		if (OverlapTags.Num() != 1)
		{
			UE_LOG(LogTemp,Log, TEXT("Please add only one Tag."))
		}
		else
		{
			if (Val > 0.8)
			{
				
				if (!CustMovementComp->IsFalling())
				{
					CustMovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::CLIMBUPLEDGE));

					ERYULedgePosition LedgePosi;

					/** Add here more cases... */
					if (OverlapTags[0] == "Above_080") LedgePosi = ERYULedgePosition::Above_080cm;
					if (OverlapTags[0] == "Above_100") LedgePosi = ERYULedgePosition::Above_100cm;;
					if (OverlapTags[0] == "Above_150") LedgePosi = ERYULedgePosition::Above_150cm;
					if (OverlapTags[0] == "Above_200") LedgePosi = ERYULedgePosition::Above_200cm;
					if (OverlapTags[0] == "climbing") LedgePosi = ERYULedgePosition::Above_450cm;
					if (OverlapTags[0] == "hurdle") LedgePosi = ERYULedgePosition::Hurdle_080cm;


					//** and of course add reactions (animations) in the BP_Character_ichi
					CustMovementComp->OnCanClimbLedge.Broadcast(LedgePosi);

				}
				
				
			}
		}

	}
}

void ARYUCharacterIchi::HangOnLedgeAndClimb(float Val)
{	
	float UpOrDown;
	//Hang on ledge && climb Up
	if (bAllowClimbUp)
	{
		if (Val > 0.8)
		{
			RYUMovement = ERYUMovementMode::CLIMB;
			RYUClimbingMode = ERYUClimbingMode::CLIMBUPLEDGE;
			CustMovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::CLIMBUPLEDGE));
			UpOrDown = -1.0f;
			ReSetHangUpPosition();
			if (ClimbAssetComp->ClimbDownMontage)
			{
				//PlayAnimMontage(ClimbAssetComp->ClimbDownMontage, UpOrDown, ClimbAssetComp->ClimbDownStart);
				PlayAnimMontage(ClimbAssetComp->ClimbUpMontage, UpOrDown, "Default");
			
			}
		}
	}
	

	//Hang on ledge && climb Down / Fall
	if (Val < -0.5)
	{
		RYUMovement = ERYUMovementMode::CLIMB;
		RYUClimbingMode = ERYUClimbingMode::FALLDOWNLEDGE;
		CustMovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::FALLDOWNLEDGE));
		UpOrDown = 1.0f;

		FHitResult HitResultGround;

		FVector LineTraceStart = GetMesh()->GetBoneLocation("ball_r");
		FVector LineTraceEnd = LineTraceStart;
		//subtract Height BigFalldownLedgeAni- Something
		LineTraceEnd.Z = LineTraceEnd.Z - 250.0f;

		AActor* ActorToIgnore = this;
		FCollisionQueryParams ColParams(FName(TEXT("LedgeHeightTracerParams")), true, ActorToIgnore);
		ColParams.bTraceComplex = true;
		ColParams.bReturnPhysicalMaterial = false;
		//ColParams.TraceTag = TraceTagFalling;

		//@ToDo Raycast nach unten if gross genug dann play anim ClimbFall else set char to _StartClimbUpPosition / later special animations ....
		bool HitGround = GetWorld()->LineTraceSingleByChannel(HitResultGround, LineTraceStart,LineTraceEnd, ECC_Visibility, ColParams);

		UE_LOG(LogTemp, Log, TEXT("LineTrace: (Start): %s (End): %s  "), *LineTraceStart.ToString(), *LineTraceEnd.ToString());
		UE_LOG(LogTemp, Log, TEXT("Impact on: %s"), *HitResultGround.ImpactPoint.ToString());

		float distanceHit = FVector::DistSquared(LineTraceStart, HitResultGround.ImpactPoint);
		float _Bla = 110.0f;

		UE_LOG(LogTemp, Log, TEXT("Sqrt(Distance) is %s: "), *FString::SanitizeFloat(FMath::Sqrt(distanceHit)));

		ReSetHangUpPosition();

		if (FMath::Sqrt(distanceHit) < _Bla)
		{
			PlayAnimMontage(ClimbAssetComp->ClimbFallMontageShort, 1.0f);

			/* START Workaround , Later climbDownani;*/
			FVector CharPosition = HitResultGround.ImpactPoint;
			CharPosition.Z = CharPosition.Z + 50;
			CustMovementComp->ResetClimbingState();
			SetActorLocation(CharPosition);
			/*END workaround*/
			
			
		}
		else
		{
			if (ClimbAssetComp->ClimbDownMontage)
			{
				PlayAnimMontage(ClimbAssetComp->ClimbFallMontage, UpOrDown);

			}
			else UE_LOG(LogTemp, Warning, TEXT("Please Link a ClimbFallMontage!"));
		}
		
	}

	

}


void ARYUCharacterIchi::CheckOverlappingComponents()
{
	for (int i = 0; i < CapsuleOverlappedComponents.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("OvAc: %s"), *CapsuleOverlappedComponents[i]->GetName());
	}
}

void ARYUCharacterIchi::SetAllowClimbUpTrue()
{
	UE_LOG(LogTemp, Log, TEXT("bAlloClimbing: true"));
	bAllowClimbUp = true;
	RYUMovement = ERYUMovementMode::CLIMB;
	RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
}

void ARYUCharacterIchi::SetAllowClimbUpFalse()
{
	UE_LOG(LogTemp, Log, TEXT("bAlloClimbing: false"));
	bAllowClimbUp = false;
}


void ARYUCharacterIchi::ResetDoOnceClimbInput()
{
	bDoOnceClimbInput = false;
}

#if WITH_EDITOR
//#include "Editor.h"
//changes in CharacterMovement are not directly applied, so to avoid making a own charactermovementcomp you need to change st in actors class to apply defaultgravityscale in editor
void ARYUCharacterIchi::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	DefaultGravityScale = GetCharacterMovement()->GravityScale;


	UE_LOG(LogTemp, Log, TEXT("Default: %s; GravityEditor: %s"), *FString::SanitizeFloat(DefaultGravityScale), *FString::SanitizeFloat(GetCharacterMovement()->GravityScale));
	UE_LOG(LogTemp, Log, TEXT("JumpZVelocity: %s"), *FString::SanitizeFloat(GetCharacterMovement()->JumpZVelocity));
}
#endif


