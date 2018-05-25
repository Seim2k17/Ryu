// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUCharacterIchi.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/RYUCustomizeMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


// Sets default values
ARYUCharacterIchi::ARYUCharacterIchi(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URYUCustomizeMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PlayerActive = ERYUPlayerActive::Player1;

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

}

// Called when the game starts or when spawned
void ARYUCharacterIchi::BeginPlay()
{
	Super::BeginPlay();
	if (CustMovementComp->GravityScaleMaximum == 0)
	{
		CustMovementComp->SetGravityScaleMaximum(MaxGravityScaleStd);
	}

	CustMovementComp->SetNormalMaxJumpCount(JumpMaxCount);

}

void ARYUCharacterIchi::Tick(float DeltaTime)
{
	currentFPS = 1 / DeltaTime;

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

		//UE_LOG(LogTemp, Log, TEXT("V(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Z)));

		if (bJumpJustStarted)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("V(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Z))), false);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("JumpZV(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(GetCharacterMovement()->JumpZVelocity))), false);
		}
	}

}


void ARYUCharacterIchi::Jump()
{
	//Super::Jump() : CharMoveComp->DoJump
	//How Long adding Velocity when maxHoldTime applied! -> maxHoldTime, but Velocity stays the same of course
	StartJumpPosition = GetActorLocation();

	//suppose wie�re not falling -> to make it a real jump not adding JumpZVelo to Vel(Z) < 0
	if (CoyoteJumpPossible)
	{
		GetCharacterMovement()->Velocity.Z = 0;
	}

	DeactivateCoyotoeJumpPossible();

	bPressedJump = true;
	
	float JumpKeyMaxTime = JumpMaxHoldTime * 1000;
	//UE_LOG(LogTemp, Log, TEXT("JumpKeyMaxTime ms: %s"), *FString::SanitizeFloat(JumpKeyMaxTime));

	float InputY = GetInputAxisValue("MoveRight");
	//@ToDo calc begin Velocity
	//add Jump-Y-Velocity aus Stand
	//UE_LOG(LogTemp, Log, TEXT("Y(start): %s"), *FString::SanitizeFloat(GetCharacterMovement()->Velocity.Y));
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
	//UE_LOG(LogTemp, Log, TEXT("Y(final): %s"), *FString::SanitizeFloat(StartJumpVelocity.Y));


	// 	//pseudocode
	// 	//pos += vel + d(t) + 1 / 2 * acc*d(t)*d(t);
	// 	//vel += acc * d(t);

	//look character Jump Sheet for customization when level up
	//Jump Z - Velocity: "wie stark sprint char in die H�he -> make it flexible (je laenger button gepresst desto hoeher springt er ); std-wert: zw. 600 und 700 --> can improve through SkillTree ?
	//std-value: 500 : Hoehe: ~1m
	//1000: JH: 2,5m
	// 670: : JH: 1,5m

	//Air control: [0..1] 0: no while falling, 1-full control at full speed -> 0.4 - 0.5 ?
}

void ARYUCharacterIchi::StopJumping()
{
	UE_LOG(LogTemp, Log, TEXT("Char: %s Release Jumping button."), *GetName());
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
		//@ToDo: better overwriting the MovementComponent from Character. --> todo so destroy it in the Constructor and add a customized version
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
		
	
		if (bJumpJustStarted == false)
		{
			TimeDeltaStart = GetWorld()->GetTimeSeconds();
			bJumpJustStarted = true;
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

			float directionV = (GetCapsuleComponent()->GetForwardVector().Y > 0) ? 1.0f : -1.0f;
			//Wenn Char schnell gelaufen ist oder aus grosser H�he gefallen, @ToDo BigHeigth
			if ((FMath::Abs(StartJumpVelocity.Y) > CustMovementComp->AfterJumpTreshold.Y) || FallCheck)
			{
				//Add little Velocity after hitting the ground
				GetCharacterMovement()->Velocity.Y = CustMovementComp->VelocityAfterJumping.Y * (directionV);
			}

			FallCheck = false;
			//if fall on the ground earlier than MaxJumpButtonHoldTime
			CoyoteJumpPossible = false;
			StopJumping();
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
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Val);
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

