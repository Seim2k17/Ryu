// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/RYUCustomizeMovementComponent.h"
#include "Character/RYUENUM_MovementMode.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"


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

	SphereTracer = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTracer"));
	SphereTracer->SetupAttachment(RootComponent);

	SphereTracer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereTracer->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereTracer->SetRelativeLocation(FVector(60, 0, 0));
	SphereTracer->SetSphereRadius(100);

	ClimbDownTracer = CreateDefaultSubobject<USphereComponent>(TEXT("ClimbDownTracer"));
	ClimbDownTracer->SetupAttachment(RootComponent);
	ClimbDownTracer->SetRelativeLocation(FVector(60, 0, 0));
	ClimbDownTracer->SetSphereRadius(10);
	


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	RYUMovement = ERYUMovementMode::STAND;
	CharacterHipSocketName = "HipSocket";

	bSphereTracerOverlap = false;

	bJumpJustStarted = false;

	TreshholdYWalkRun = 220.0f;

	ClimbDownTraceLength = 80.0f;

}


ARYUCharacterBase::ARYUCharacterBase(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<URYUCustomizeMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	SphereTracer = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTracer"));
	SphereTracer->SetupAttachment(RootComponent);

	SphereTracer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereTracer->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereTracer->SetRelativeLocation(FVector(60, 0, 0));
	SphereTracer->SetSphereRadius(100);

	ClimbDownTracer = CreateDefaultSubobject<USphereComponent>(TEXT("ClimbDownTracer"));
	ClimbDownTracer->SetupAttachment(RootComponent);
	ClimbDownTracer->SetRelativeLocation(FVector(60, 0, 0));
	ClimbDownTracer->SetSphereRadius(10);

	ClimbDownTraceLength = 120.0f;

	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void ARYUCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//NEED TO BIND IT IN THE DERIVED CLASSES !!!! (it works in BeginPlay but not in constructor: thats why in an existed Character-BP: AddDynamic gets not registered! 
	//--> entweder you need to make a new Char-BP then AddDynamic works in contructor or better you use register the Overlap Method (Adddynamic) in BeginPlay)
	// 	SphereTracer->OnComponentBeginOverlap.AddDynamic(this, &ARYUCharacterBase::OnSphereTracerHandleBeginOverlap);
	// 	SphereTracer->OnComponentEndOverlap.AddDynamic(this, &ARYUCharacterBase::OnSphereTracerHandleEndOverlap);

	GetWorld()->DebugDrawTraceTag = TraceTag;
}

void ARYUCharacterBase::MoveRight(float Val)
{

	if (FMath::Abs(GetCharacterMovement()->Velocity.Y) > 0)
	{
		if (FMath::Abs(GetCharacterMovement()->Velocity.Y) > TreshholdYWalkRun)
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
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Val);
}

void ARYUCharacterBase::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	//Jump();
}

void ARYUCharacterBase::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	//StopJumping();
}


// Called every frame
void ARYUCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//doesn´t work properly FUCKSHITSUCKER
	//CheckClimbDownTracer();
}


void ARYUCharacterBase::Jump()
{
	Super::Jump();

	RYUMovement = ERYUMovementMode::JUMP;
}

void ARYUCharacterBase::StopJumping()
{
	Super::ResetJumpState();
}

void ARYUCharacterBase::CheckLedgeTracer()
{
	//UE_LOG(LogTemp, Log, TEXT("CheckLedgeTracer"));
	switch (RYUMovement)
	{
		case ERYUMovementMode::CANTRACELEDGE:
		{
			TraceHeightAndWallOfLedge();
			break;
		}
		
		case ERYUMovementMode::CANGRABLEDGE:
		{
			CheckClimbingLedge();
			break;
		}
		
		case ERYUMovementMode::HANGONLEDGE:
			return;
		case ERYUMovementMode::CLIMBDOWNLEDGE:
			return;
		default:
			break;
	}
}

//** did a 1:1 implementation of the BP Version in the Sidescroller.BP
void ARYUCharacterBase::TraceHeightAndWallOfLedge()
{
	//UE_LOG(LogTemp, Log, TEXT("SphereTracer: Search Height"));
	//1.rst SphereTracer Height 50Units


	AActor* ActorToIgnore = this;
	FCollisionQueryParams ColParams(FName(TEXT("LedgeHeightTracerParams")), true, ActorToIgnore);
	ColParams.bTraceComplex = true;
	ColParams.bReturnPhysicalMaterial = false;

	FHitResult HitresultHeight;

	//** see Macro "Ledge Tracer in Sidescroller.BP 
	FVector SweepStartHeight;
	FVector SweepEndHeight;
	FQuat SweepRotHeight = GetActorQuat();

	//cal. Start and End
	float StartHeight = 500;
	float EndHeight = 550;
	SweepStartHeight = GetActorLocation();
	SweepStartHeight.Z = SweepStartHeight.Z + StartHeight;

	FVector CharForward = GetActorForwardVector();
	CharForward = CharForward * 70.0f;
	

	SweepStartHeight = SweepStartHeight + CharForward;

	SweepEndHeight = SweepStartHeight;
	SweepEndHeight.Z = SweepEndHeight.Z - EndHeight;
// Macro End


/************************************************************************/
/* see DefaultEngine.ini:
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel1,Name="SphereTracer",DefaultResponse=ECR_Ignore,bTraceType=False,bStaticObject=False)
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,Name="LedgeTrace",DefaultResponse=ECR_Ignore,bTraceType=True,bStaticObject=False)
*/
/************************************************************************/
//A: if there is a possible ledge Hit in Height 
//@ToDo: If there are more than one ledges layered -> takes the above --> FAIL (=can´t climb)
bool HitLedgeHeight = GetWorld()->SweepSingleByChannel(HitresultHeight, SweepStartHeight, SweepEndHeight, SweepRotHeight,
	ECollisionChannel::ECC_EngineTraceChannel2, FCollisionShape::MakeSphere(2), ColParams);

if (HitLedgeHeight)
{
	LedgeTracerHeight = HitresultHeight.ImpactPoint;
	DrawDebugSphere(GetWorld(), LedgeTracerHeight, 5, 10, FColor::Green, false, 0.0f, 0, 0.0f);
	/** Check if Ledge is in Height */
	HipSocketLocation = GetMesh()->GetSocketLocation(CharacterHipSocketName);
	float HipSocketZ = HipSocketLocation.Z;
	float LedgeTracerHeightZ = LedgeTracerHeight.Z;

	if (FMath::IsWithinInclusive((HipSocketZ - LedgeTracerHeightZ), -200.0f, 0.0f))
	{
		//if (!bLedgeTraceInRangeChanged)
		{
			UE_LOG(LogTemp, Log, TEXT("LedgeHeigth: %s"), *LedgeTracerHeight.ToString());
			bLedgeTraceNotInRangeChanged = false;
			//CAUTION when Ledges Overlap ! Maybe a pimp is needable !
			bLedgeTraceInRangeChanged = true;
			UE_LOG(LogTemp, Log, TEXT("LedgeHeigth in Range"));
			bLedgeHeightInRange = true;

			//done in Tick (ichi) --> move to CharBase ?
			RYUMovement = ERYUMovementMode::CANGRABLEDGE;
		}
	}
	else {
		//if (!bLedgeTraceNotInRangeChanged)
		{
			UE_LOG(LogTemp, Log, TEXT("LedgeHeigth: %s"), *LedgeTracerHeight.ToString());
			bLedgeTraceNotInRangeChanged = true;
			bLedgeTraceInRangeChanged = false;
			UE_LOG(LogTemp, Log, TEXT("LedgeHeigth NOT in Range"));
			bLedgeHeightInRange = false;

			//done in Tick (ichi) --> move to CharBase ?
			RYUMovement = ERYUMovementMode::CANTRACELEDGE;
		}
	}
}
else
{
	//Reset Stuff
	bLedgeTraceInRangeChanged = false;
	bLedgeHeightInRange = false;
	bLedgeTraceNotInRangeChanged = false;
	bLedgeTracePossible = false;
}

bLedgeTracePossible = HitLedgeHeight;

//** see Macro "Wall Tracer in Sidescroller.BP 
FHitResult HitresultWall;
FVector SweepStartWall;
FVector SweepEndWall;
FQuat SweepRotWall = GetActorQuat();

SweepStartWall = GetActorLocation();
FVector CharForwardWall = GetActorForwardVector();
CharForwardWall.X = CharForwardWall.X * 150.0f;
CharForwardWall.Y = CharForwardWall.Y * 150.0f;
SweepEndWall = SweepStartWall + CharForwardWall;

//Macro End

//B: if there is a possible ledge Hit in a Wall infront
bool HitLedgeWall = GetWorld()->SweepSingleByChannel(HitresultWall, SweepStartWall, SweepEndWall, SweepRotWall, ECollisionChannel::ECC_EngineTraceChannel2, FCollisionShape::MakeSphere(20), ColParams);

if (HitLedgeWall)
{
	LedgeTracerWall = HitresultWall.ImpactPoint;
	LedgeTracerWallNormal = HitresultWall.Normal;
	DrawDebugSphere(GetWorld(), LedgeTracerWall, 5, 10, FColor::Green, false, 0.0f, 0, 0.0f);
}
}

void ARYUCharacterBase::CheckClimbingLedge()
{
	//BaseClassImplementation
	UE_LOG(LogTemp, Log, TEXT("Can Climb ledge from BaseClass, please override in your Deriving Class"));
}


void ARYUCharacterBase::CheckClimbDownTracer()
{
	
	{
		if (GetMovementComponent()->IsMovingOnGround())
		{
			FHitResult ClimbDownHitResult;
			FVector TraceStart = ClimbDownTracer->GetComponentLocation();
			FVector TraceEnd = TraceStart;
			TraceEnd.Z = TraceEnd.Z - ClimbDownTraceLength;

			FCollisionQueryParams QueryParams;
			QueryParams.TraceTag = TraceTag;
			QueryParams.bTraceComplex = true;

			bool HitClimbDown = GetWorld()->LineTraceSingleByChannel(ClimbDownHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);

			if (ClimbDownHitResult.bBlockingHit == false)
			{
				RYUMovement = ERYUMovementMode::CANCLIMBDOWNLEDGE;
			}
		}

	}
	
	
}

void ARYUCharacterBase::OnSphereTracerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr)
	{
		SphereOverlappedActor = OtherActor;
		bSphereTracerOverlap = true;
		UE_LOG(LogTemp, Log, TEXT("SphereTracer Overlap In with: %s"),*SphereOverlappedActor->GetName());
		if ((RYUMovement != ERYUMovementMode::CANCLIMBDOWNLEDGE) &&
			(RYUMovement != ERYUMovementMode::CLIMBDOWNLEDGE) &&
			(RYUMovement != ERYUMovementMode::CLIMBUPLEDGE) &&
			(RYUMovement != ERYUMovementMode::HANGONLEDGE))
		{
			RYUMovement = ERYUMovementMode::CANTRACELEDGE;
		}

	}

}


void ARYUCharacterBase::OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSphereTracerOverlap = false;
	bLedgeHeightInRange = false;
	//SphereOverlappedActor = nullptr;
	UE_LOG(LogTemp, Log, TEXT("SpherTracer Overlap Out"));
	if ((RYUMovement != ERYUMovementMode::CANCLIMBDOWNLEDGE) &&
		(RYUMovement != ERYUMovementMode::CLIMBDOWNLEDGE) &&
		(RYUMovement != ERYUMovementMode::CLIMBUPLEDGE) &&
		(RYUMovement != ERYUMovementMode::HANGONLEDGE))

	{
		RYUMovement = ERYUMovementMode::WALK;
	}
}