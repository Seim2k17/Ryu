// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/RYUCustomizeMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


// Sets default values
ARYUCharacterBase::ARYUCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CustMovementComp = CreateDefaultSubobject<URYUCustomizeMovementComponent>(TEXT("RYUCustomizeMovementData"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	
	PlayerActive = ERYUPlayerActive::Player1;

	InitializeCharacterValues();

}

void ARYUCharacterBase::InitializeCharacterValues()
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
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->JumpZVelocity = 650.0f;
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

	StartJumpZVelocity = GetCharacterMovement()->JumpZVelocity;

	tmpCheck = false;
}

// Called when the game starts or when spawned
void ARYUCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (CustMovementComp->GravityScaleMaximum == 0)
	{
		CustMovementComp->SetGravityScaleMaximum(MaxGravityScaleStd);
	}

	
}

void ARYUCharacterBase::MoveRight(float Val)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Val);
}

void ARYUCharacterBase::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ARYUCharacterBase::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}


// Called every frame
void ARYUCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentFPS = 1 / DeltaTime;

	if (bDebugOutputActive)
	{
		DrawDebugInfosOnScreen();
	}

	//check preferences after Jump is pressed
	AfterJumpButtonPressed();
}



void ARYUCharacterBase::DrawDebugInfosOnScreen()
{

	currA = GetCharacterMovement()->GetCurrentAcceleration();
	currV = this->GetVelocity();
  	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("V(x): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.X))),false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("V(y): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Y))),false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("V(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Z))),false);

		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("a(x): %s"), *FString::SanitizeFloat(currA.X)), false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("a(y): %s"), *FString::SanitizeFloat(currA.Y)), false);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("a(z): %s"), *FString::SanitizeFloat(currA.Z)), false);

		UE_LOG(LogTemp, Log, TEXT("V(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Z)));

		if (bJumpJustStarted)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("V(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(currV.Z))), false);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("JumpZV(z): %s"), *FString::SanitizeFloat(FMath::RoundToFloat(GetCharacterMovement()->JumpZVelocity))), false);
		}
	}
	
}

void ARYUCharacterBase::Jump()
{
 	//UE_LOG(LogTemp, Log, TEXT("Char: %s Starts Jumping"), *GetName());
	
	//Super::Jump() :
	//CharMoveComp->DoJump(

	//@ToDo: Check How Long adding Velocity when maxHoldTime applied!
	//TimeDelta = GetSecond

	bPressedJump = true;
	
	float InputY = GetInputAxisValue("MoveRight");
	GetCharacterMovement()->Velocity.Y = CustMovementComp->JumpForce.Y * InputY * (-1.0f);
	UE_LOG(LogTemp, Log, TEXT("Y: %s"), *FString::SanitizeFloat(InputY));

	float JumpKeyMaxTime = JumpMaxHoldTime * 1000;
	UE_LOG(LogTemp, Log, TEXT("JumpKeyMaxTime ms: %s"), *FString::SanitizeFloat(JumpKeyMaxTime));
	
	//Jumping inits
	if (JumpMaxHoldTime > 0.0f)
	{
		//normal add 1 Frame JumpZ:
		//jetzt X Frames auf Dauer MaxHoldTime -> 
		float JZFrames = currentFPS * JumpMaxHoldTime;
		//GetCharacterMovement()->JumpZVelocity = StartJumpZVelocity / JZFrames;
		//UE_LOG(LogTemp, Log, TEXT("JumpZVeloc: %s over %s Frames."), *FString::SanitizeFloat(GetCharacterMovement()->JumpZVelocity),*FString::SanitizeFloat(JZFrames));
		//GetCharacterMovement()->JumpZVelocity = GetCharacterMovement()->JumpZVelocity / CustMovementComp->JumpHoldDivider.Y;
	}


// 	//pseudocode
// 	//pos += vel + d(t) + 1 / 2 * acc*d(t)*d(t);
// 	//vel += acc * d(t);

//look character Jump Sheet for customization when level up
//Jump Z - Velocity: "wie stark sprint char in die Höhe -> make it flexible (je laenger button gepresst desto hoeher springt er ); std-wert: zw. 600 und 700 --> can improve through SkillTree ?
//std-value: 500 : Hoehe: ~1m
//1000: JH: 2,5m
// 670: : JH: 1,5m

//Air control: [0..1] 0: no while falling, 1-full control at full speed -> 0.4 - 0.5 ?

}

void ARYUCharacterBase::StopJumping()
{
	UE_LOG(LogTemp, Log, TEXT("Char: %s Release Jumping button."), *GetName());
	float JumpKeyTime = JumpKeyHoldTime * 1000;
	UE_LOG(LogTemp, Log, TEXT("JumpKeypressed ms: %s"), *FString::SanitizeFloat(JumpKeyTime));
	UE_LOG(LogTemp, Log, TEXT("JumpKeyCount: %s"), *FString::FromInt(JumpCurrentCount));
	//Super::StopJumping();
	bPressedJump = false;
	
	Super::ResetJumpState();
	
}

//called in Tick
void ARYUCharacterBase::AfterJumpButtonPressed()
{
	//we suppose Jumping occurred
	if (GetCharacterMovement()->IsMovingOnGround() == false)
	{
		if (bJumpJustStarted == false) bJumpJustStarted = true;
		if (GetVelocity().Z < 0)
		{
			if (tmpCheck == false)
			{
				UE_LOG(LogTemp, Log, TEXT("Peak reached"), *FString::FromInt(JumpCurrentCount));
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
		

		
	}
	else
	{
		if (bJumpJustStarted)
		{
			//@ToDo: check direction (ForwardVector)
			float directionV = (GetCapsuleComponent()->GetForwardVector().Y > 0) ? 1.0f : -1.0f;

			//Add little Velocity after hitting the ground
			GetCharacterMovement()->Velocity.Y = CustMovementComp->VelocityAfterJumping.Y * (directionV);
			bJumpJustStarted = false;
			//GetCharacterMovement()->GravityScale = DefaultGravityScale;
		}
		return;
	}

}



#if WITH_EDITOR
//#include "Editor.h"
//changes in CharacterMovement are not directly applied, so to avoid making a own charactermovementcomp you need to change st in actors class to apply defaultgravityscale in editor
void ARYUCharacterBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	DefaultGravityScale = GetCharacterMovement()->GravityScale;
	GetCharacterMovement()->JumpZVelocity = GetCharacterMovement()->JumpZVelocity / CustMovementComp->JumpHoldDivider.Y;

	UE_LOG(LogTemp, Log, TEXT("Default: %s; GravityEditor: %s"),*FString::SanitizeFloat(DefaultGravityScale), *FString::SanitizeFloat(GetCharacterMovement()->GravityScale));
	UE_LOG(LogTemp, Log, TEXT("JumpZVelocity: %s"), *FString::SanitizeFloat(GetCharacterMovement()->JumpZVelocity));
}
#endif

//////////////////////////////////////////////////////////////////////////
// Input


// Called to bind functionality to input
void ARYUCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	//Axis-Bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ARYUCharacterBase::MoveRight);

	//Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARYUCharacterBase::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARYUCharacterBase::StopJumping);
	
	PlayerInputComponent->BindAction("ChangePlayer",IE_Pressed, this, &ARYUCharacterBase::ChangePlayer);


	//Predefined Bindings
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARYUCharacterBase::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARYUCharacterBase::TouchStopped);
}

void ARYUCharacterBase::ChangePlayer()
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




