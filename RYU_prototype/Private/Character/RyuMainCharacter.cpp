// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuMainCharacter.h"
#include "Components/RyuClimbingComponent.h"
#include "Components/RyuDebugComponent.h"
#include "Components/RyuMovementComponent.h"
#include "Components/RyuTimelineComponent.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMovementState.h"
#include "States/RyuCharacterIdleState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYUClimbingActor.h"
#include "RYU_prototype.h"
#include "RyuCharacterState.h"
#include <DrawDebugHelpers.h>
#include <PaperFlipbookComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <Curves/CurveVector.h>
#include <GameFramework/Character.h>
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/GameplayStatics.h>
#include <PaperZD/Public/AnimSequences/Players/PaperZDAnimPlayer.h>
#include <PaperZD/Public/PaperZDAnimBP.h>
#include <PaperZD/Public/PaperZDAnimInstance.h>

ARyuMainCharacter::ARyuMainCharacter(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<URyuMovementComponent>(
        ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    // Holds Data and Links to Curves which follows a Timeline
    RyuTimelineComponent =
        CreateDefaultSubobject<URyuTimelineComponent>(TEXT("RyuTimeLineComponent"));

    InitializeCharacterValues();
}

void ARyuMainCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    //TODO: take closer look when CustomMovement will be impl.
}

#if WITH_EDITOR
void ARyuMainCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // refresh DebugState
    if (auto* DebugComp = FindComponentByClass<URyuDebugComponent>())
    {
        DebugComp->PostEditChangePropertyFromOwner();
    }

    if (bLineTracingActive)
    {
        SetDebuggedCharacter();
    }
}
#endif

void ARyuMainCharacter::InitializeCharacterValues()
{
    // Use only Yaw from the controller and ignore the rest of the rotation.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    // Enable replication on the Sprite component so animations show up when networked
    GetSprite()->SetIsReplicated(true);
    bReplicates = true;

    bPlayTurnAni = false;
    SetLookDirection(ERyuLookDirection::Right);
    bLookRight = true;

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

    /* Hmm set Values only at initializing*/
    CameraBoom->TargetArmLength = 500.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
    CameraBoom->SetUsingAbsoluteRotation(true);
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->SetRelativeRotation(FRotator(.0f, -90.0f, 0.0f));

    // Prevent all automatic rotation behavior on the camera, character, and camera component
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

    bSneakIsPressed = false;
}

float ARyuMainCharacter::MoveRightKeyStatus()
{
    UE_LOG(LogRyu, Error, TEXT("MoveRightKeyStatus is called"));
    return 0;
}

void ARyuMainCharacter::SetDebuggedCharacter()
{
    TraceStart = this->GetActorLocation();
    TraceEnd = TraceStart - (LengthLineTrace * this->GetActorUpVector());
    IgnoredActors.Add(this);
    TraceTag = TEXT("Debug");
    CollisionParams.AddIgnoredActors(IgnoredActors);
    CollisionParams.bTraceComplex = true;
    CollisionParams.TraceTag = TraceTag;

    TheWorld = GetWorld();
}

void ARyuMainCharacter::StartLineTracing()
{
    // DrawDebugSphere(TheWorld, GetActorLocation(), 10.0f, 10, FColor::Red, false, 1.f, 0, 2.f);
    TraceStart = this->GetActorLocation();
    TraceEnd = TraceStart - (LengthLineTrace * this->GetActorUpVector());
    FCollisionShape HitSphere = FCollisionShape::MakeSphere(this->HitSphereRaduis);

    //TraceEnd = TraceStart - (LengthLineTrace * (-1.f) * this->GetActorForwardVector());

    // TODO: set own CollisionChannel to LineTrace! -> but BSPs can´t have one so for testing we NEED to See the the BSP-Geometry and tht the CollChannel to Visibility !
    TheWorld->SweepSingleByChannel(CharHitResult, TraceStart, TraceEnd, FQuat::Identity,
                                   ECollisionChannel::ECC_Visibility, HitSphere, CollisionParams);
    //TheWorld->LineTraceSingleByChannel(CharHitResult, TraceStart, TraceEnd,
    //                                   ECollisionChannel::ECC_Visibility, CollisionParams);

    if (bLineTracingVisible)
    {
        DrawDebugSphere(TheWorld, TraceEnd, HitSphere.GetSphereRadius(), 10, FColor::Green, false,
                        0.1f);
        // DrawDebugLine(TheWorld, TraceStart, TraceEnd, FColor::Red, false, -1.f, 0, 2.0f);
    }
}

void ARyuMainCharacter::BeginPlay()
{
    Super::BeginPlay();

    //do we still need it
    //GetSprite()->OnFinishedPlaying.AddDynamic(this, &ARYU2D_CharacterPrince::FlipbookFinishedPlaying);

    SphereTracer->OnComponentBeginOverlap.AddDynamic(
        this, &ARyuMainCharacter::HandleSphereColliderBeginOverlap);
    SphereTracer->OnComponentEndOverlap.AddDynamic(
        this, &ARyuMainCharacter::HandleSphereColliderEndOverlap);

    CharacterState = NewObject<URyuCharacterIdleState>();

    if (bLineTracingActive)
    {
        SetDebuggedCharacter();
    }
}

void ARyuMainCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Temp save for Timelinestuff
    fDeltaSeconds = DeltaTime;

    //TODO implement it correct then call it / atm crash -> pure virtual function call !
    //TODO TESTING here sometimes crash ? see screenshot #090719_StateUpdate / #060819_StateUpdate (FocusLost / ALT_TAB)
    // PLEASE Check if InputPressed: ReleaseInputKey" overrides "StateEndedState!" -> e.g.: JumpPressed -> AniJump -> JumpAniEnded -> EndStateCalled but before when ReleaseJumpKey is triggered: InputPressed == None .... ???!!!???
    // Unhandled exception

    // UE_LOG(LogRyu, Log, TEXT("TickIntervall@MainChar: %f"), this->PrimaryActorTick.TickInterval);

    //     if ((CharacterState != nullptr) && (CharacterState->GetState())
    //         && (CharacterState->GetInputPressedState() != ERyuInputState::None))
    //     {
    //         CharacterState->Update(this);
    //     }

    //** due its better for complexicity AND clarity we do most of the ABP_Transition_Logic here in c++
    // TODO: integrate this stuff in the new CharacterStateMachine !

    //UpdateCharacter();a

    if (bLineTracingActive)
    {
        StartLineTracing();
    }
}

//TODO Movecompletely to BaseCharacter when everythings works
void ARyuMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
    //PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    //PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARyuMainCharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARyuMainCharacter::StopJumping);
    PlayerInputComponent->BindAxis("MoveRight", this, &ARyuMainCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Climb", this, &ARyuMainCharacter::MoveUp);
    PlayerInputComponent->BindAction("Sneak", IE_Pressed, this, &ARyuMainCharacter::SneakPressed);
    PlayerInputComponent->BindAction("Sneak", IE_Released, this, &ARyuMainCharacter::SneakReleased);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARyuMainCharacter::SprintPressed);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this,
                                     &ARyuMainCharacter::SprintReleased);
}

void ARyuMainCharacter::Jump()
{
    if (GetRyuCharacterMovement()->IsAllowedToJump())
    {
        // TODO: is this even executed atm ? see SetupPlayerInputComponent --> yes it is
        // from CharacterClass
        if (!bJumpJustStarted)
        {
            UE_LOG(LogRyu, Warning, TEXT("Jump from MainChar called."));
            bPressedJump = true;
            bJumpJustStarted = true;
            // just for testing deactivate JumpCSM, to recheck if everything BASIC works!
            ARyuBaseCharacter::HandleInput(ERyuInputState::PressJump);
        }
    }
}

void ARyuMainCharacter::HandleSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                         AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp,
                                                         int32 OtherBodyIndex, bool bFromSweep,
                                                         const FHitResult& SweepResult)
{
    //TODO: needed ?
}

void ARyuMainCharacter::HandleSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                       AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp,
                                                       int32 OtherBodyIndex)
{
    //TODO: needed ?
}

void ARyuMainCharacter::StopJumping()
{
    bPressedJump = false;

    bJumpJustStarted = false;

    // TODO: How To Hndle State ReleaseJump ? ARyuBaseCharacter::HandleInput(ERyuInputState::ReleaseJump);

    Super::ResetJumpState();
}

//Move Left or Right
void ARyuMainCharacter::MoveRight(float Val)
{
    //for slow Movement , TODO: Clamp for ControllerInput (AxisValus...)

    bool PressedRight = false;

    float SneakValue = 1.0f;

    bSneakIsPressed ? SneakValue = RyuMovementComponent->GetSneakMultiplier() : SneakValue = 1.0f;

    MoveRightInput = Val * SneakValue;

    if (bSneakIsPressed)
    {
        //HandleInput(ERyuInputState::PressSneak);
    }
    if (MoveRightInput < 0)
    {
        PressedRight = false;
        if (ARyuBaseCharacter::GetMoveRightAxisState()
            != ERyuMoveRightAxisInputState::PressLeftAxisKey)
        {
            ARyuBaseCharacter::HandleInput(ERyuInputState::PressLeft);
        }
    }
    else if (MoveRightInput > 0)
    {
        PressedRight = true;

        if (ARyuBaseCharacter::GetMoveRightAxisState()
            != ERyuMoveRightAxisInputState::PressRightAxisKey)
        {
            ARyuBaseCharacter::HandleInput(ERyuInputState::PressRight);
        }
    }
    else
    {
        // TODO: RECHECK: WHAT DOES IT MEAN quickly changeing direction ???
        if (ARyuBaseCharacter::GetMoveRightAxisState() != ERyuMoveRightAxisInputState::Inactive)
        {
            // TODO: check is needable (ReleaseAxisKey .... / or only switch to inactive when release is done, is it executed in one frame ?
            // MoveRightAxisState = ERyuMoveRightAxisInputState::ReleaseRightAxisKey;
            if (ARyuBaseCharacter::GetMoveRightAxisState()
                == ERyuMoveRightAxisInputState::PressRightAxisKey)
            {
                if (bAllowReleaseAxisKey)
                {
                    ARyuBaseCharacter::HandleInput(ERyuInputState::ReleaseRight);
                }
            }
            else if (ARyuBaseCharacter::GetMoveRightAxisState()
                     == ERyuMoveRightAxisInputState::PressLeftAxisKey)
            {
                if (bAllowReleaseAxisKey)
                {
                    ARyuBaseCharacter::HandleInput(ERyuInputState::ReleaseLeft);
                }
            }
        }
    }
}

//Move Up or Down
void ARyuMainCharacter::MoveUp(float Value)
{
    //Class-global MoveUpAxis Input to use in several Methods inside the MainCharClass
    MoveUpInput = Value;
}

// TODO see if still needed here when CSTM is established !
//* is called in MoveRight(Val), not in Tick anymore, but as MoveRight is bound to an axis this Ticks also
void ARyuMainCharacter::UpdateCharacter()
{
    FVector currA = GetCharacterMovement()->GetCurrentAcceleration();
    FVector currV = this->GetVelocity();

    bool bIsinAir = GetCharacterMovement()->IsFalling();

    FString bla = "";
    bIsinAir ? bla = "Char is in Air: true" : "false";

    UE_LOG(LogTemp, Log, TEXT("%s"), *bla);
}

void ARyuMainCharacter::Climb()
{
    RyuClimbingComponent->Climb(MoveUpInput);
}

/** Getter and Setter*/

// TODO CSTM relevant
void ARyuMainCharacter::ClimbLedgeFlipBookFinished()
{
    UE_LOG(LogTemp, Log,
           TEXT("ClimbLedgeFlipBookFinished(): Call From Notify->ClimbUpFlippbookFinished"));

    ERYUClimbingMode RYUClimbingMode = RyuClimbingComponent->GetClimbingState();

    switch (RYUClimbingMode)
    {
        case ERYUClimbingMode::CLIMBDOWNLEDGE:
        {
            RyuClimbingComponent->SetClimbingState(ERYUClimbingMode::HANGONLEDGE);
            //RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
        }
        break;
        case ERYUClimbingMode::CLIMBUPLEDGE:
        {
            SetActorLocation(RyuClimbingComponent->ClimbStandUpPosition);
            ResetCollisionAndGravity();
            break;
        }
        case ERYUClimbingMode::LETGOLEDGE:
        {
            UE_LOG(LogTemp, Log, TEXT("ClimbLedgeFlipBookFinished(): LETGOEND"));
            ResetCollisionAndGravity();
            break;
        }

        default:
            break;
    }
}

void ARyuMainCharacter::ResetCollisionAndGravity()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereTracer->SetEnableGravity(true);
    GetCapsuleComponent()->SetEnableGravity(true);
    RyuMovementComponent->SetMovementMode(MOVE_Walking);
    CheckOverlapClimbableActors();
}

void ARyuMainCharacter::TurnRunFlipBookFinished()
{
    FlipCharacter();

    //TODO CSTM    PlayerMovement = EPlayerMovement::RUN;
}

bool ARyuMainCharacter::CheckFlipOverlappedActor(UBoxComponent* ClimbingTrigger)
{
    //TODO: search correct actor, do we need the ArrowComponent ? guess it´s only for EditorPurposes

    if (ClimbingTrigger)
    {
        UE_LOG(LogTemp, Log, TEXT("CheckFlipOverlappedActor(): execute."));

        ARYUClimbingActor* ARYCA = Cast<ARYUClimbingActor>(ClimbingTrigger->GetOwner());
        if (ARYCA)
        {
            UArrowComponent* TriggerArrow = ARYCA->ClimbingTriggerDirection;

            //float DudePitch = this->GetArrowComponent()->GetComponentRotation().Pitch;

            // TODO ReCheck ! why no Compile `?
            // float DudePitch = 180.0f; // just test because getarrow does not compile
            float DudePitch = GetActorForwardVector().Rotation().Yaw;
            DudePitch = FMath::CeilToFloat(FMath::Abs(DudePitch));
            //FMath::CeilToFloat(DudePitch);
            UE_LOG(LogTemp, Log,
                   TEXT("CheckFlipOverlappedActor(): PitchTriggerActorArrow: %s PitchClimbingDude: "
                        "%f, Ceiled(Abs,PitchDude): %s"),
                   *TriggerArrow->GetComponentRotation().ToString(),
                   GetActorForwardVector().Rotation().Yaw, *FString::SanitizeFloat(DudePitch));
            if (TriggerArrow && (TriggerArrow->GetComponentRotation().Yaw == DudePitch))
            {
                UE_LOG(LogTemp, Log,
                       TEXT("CheckFlipOverlappedActor(): Yes it´s a climbing trigger."));
                return true;
            }
        }
    }
    return false;
}

float ARyuMainCharacter::GetMoveRightInput()
{
    return MoveRightInput;
}

float ARyuMainCharacter::GetMoveUpInput()
{
    return MoveUpInput;
}

float ARyuMainCharacter::GetFallingMoveRightMultiplier()
{
	return(GetRyuCharacterMovement()->FallingMoveRightMultiplier);
}

bool ARyuMainCharacter::GetSneakActive()
{
    return bSneakIsPressed;
}

FHitResult ARyuMainCharacter::GetHitResult()
{
    return CharHitResult;
}

void ARyuMainCharacter::SneakPressed()
{
    SneakMultiplierValue = RyuMovementComponent->GetSneakMultiplier();
    bSneakIsPressed = true;
    SetCharacterMovementState(ERyuMovementState::Sneaking);
}

void ARyuMainCharacter::SneakReleased()
{
    SneakMultiplierValue = 1.0f;
    bSneakIsPressed = false;
    SetCharacterMovementState(ERyuMovementState::Standing);
}

void ARyuMainCharacter::SprintPressed()
{
    SetCharacterMovementState(ERyuMovementState::Sprinting);
}

void ARyuMainCharacter::SprintReleased()
{
    if (bSneakIsPressed)
    {
        SetCharacterMovementState(ERyuMovementState::Sneaking);
    }
    else
    {
        SetCharacterMovementState(ERyuMovementState::Running);
    }
}

/** CHaracter State Machine Prep from Gollum

void ARyuMainCharacter::TickStates(float DeltaTime)
{
	switch (State)
	{
		case ERyuMainCharacterStates::Spawning:
			OnSpawningTick(DeltaTime);
			break;
		case ERyuMainCharacterStates::Idle:
			OnIdleTick(DeltaTime);
			break;
		case ERyuMainCharacterStates::Walking:
			OnWalkingTick(DeltaTime);
			break;
		case ERyuMainCharacterStates::Running:
			OnRunningTick(DeltaTime);
			break;
		case ERyuMainCharacterStates::Attacking:
			OnAttackingTick(DeltaTime);
			break;
		case ERyuMainCharacterStates::Dying:
			OnDyingTick(DeltaTime);
			break;
		case ERyuMainCharacterStates::Dead:
			OnDeadTick(DeltaTime);
			break;
		default:
			break;
	}
}

void ARyuMainCharacter::OnSpawningEnter()
{
	check(State == ERyuMainCharacterStates::Spawning);
}

void ARyuMainCharacter1::OnSpawningTick(float DeltaTime)
{
	check(State == ERyuMainCharacterStates::Spawning);
	TransitionSpawningToIdle();
}

void ARyuMainCharacter1::OnSpawningExit()
{
	check(State == ERyuMainCharacterStates::Spawning);
}

void ARyuMainCharacter1::TransitionSpawningToIdle()
{
	OnSpawningExit();
	State = ERyuMainCharacterStates::Idle;
	OnIdleEnter();
}

void ARyuMainCharacter1::OnIdleEnter()
{
	check(State == ERyuMainCharacterStates::Idle);
}

void ARyuMainCharacter1::OnIdleTick(float DeltaTime)
{
	check(State == ERyuMainCharacterStates::Idle);
	if (ShouldDie())
	{
		TransitionIdleToDying();
	}
	else if (ShouldAttack())
	{
		TransitionIdleToAttacking();
	}
	else if (ShouldWalk())
	{
		TransitionIdleToWalking();
	}
}

void ARyuMainCharacter1::OnIdleExit()
{
	check(State == ERyuMainCharacterStates::Idle);
}

void ARyuMainCharacter1::TransitionIdleToWalking()
{
	OnIdleExit();
	State = ERyuMainCharacterStates::Walking;
	OnWalkingEnter();
}

void ARyuMainCharacter1::TransitionIdleToAttacking()
{
	OnIdleExit();
	State = ERyuMainCharacterStates::Attacking;
	OnAttackingEnter();
}

void ARyuMainCharacter1::TransitionIdleToDying()
{
	OnIdleExit();
	State = ERyuMainCharacterStates::Dying;
	OnDyingEnter();
}

void ARyuMainCharacter1::OnWalkingEnter()
{
	check(State == ERyuMainCharacterStates::Walking);
}

void ARyuMainCharacter1::OnWalkingTick(float DeltaTime)
{
	check(State == ERyuMainCharacterStates::Walking);
	if (ShouldDie())
	{
		TransitionWalkingToDying();
	}
	else if (ShouldAttack())
	{
		TransitionWalkingToAttacking();
	}
	else if (ShouldIdle())
	{
		TransitionWalkingToIdle();
	}
	else if (ShouldRun())
	{
		TransitionWalkingToRunning();
	}
}

void ARyuMainCharacter1::OnWalkingExit()
{
	check(State == ERyuMainCharacterStates::Walking);
}

void ARyuMainCharacter1::TransitionWalkingToIdle()
{
	OnWalkingExit();
	State = ERyuMainCharacterStates::Idle;
	OnIdleEnter();
}

void ARyuMainCharacter1::TransitionWalkingToRunning()
{
	OnWalkingExit();
	State = ERyuMainCharacterStates::Running;
	OnRunningEnter();
}

void ARyuMainCharacter1::TransitionWalkingToAttacking()
{
	OnWalkingExit();
	State = ERyuMainCharacterStates::Attacking;
	OnAttackingEnter();
}

void ARyuMainCharacter1::TransitionWalkingToDying()
{
	OnWalkingExit();
	State = ERyuMainCharacterStates::Dying;
	OnDyingEnter();
}

void ARyuMainCharacter1::OnRunningEnter()
{
	check(State == ERyuMainCharacterStates::Running);
}

void ARyuMainCharacter1::OnRunningTick(float DeltaTime)
{
	check(State == ERyuMainCharacterStates::Running);
	if (ShouldDie())
	{
		TransitionRunningToDying();
	}
	else if (ShouldAttack())
	{
		TransitionRunningToAttacking();
	}
	else if (ShouldWalk())
	{
		TransitionRunningToWalking();
	}
}

void ARyuMainCharacter1::OnRunningExit()
{
	check(State == ERyuMainCharacterStates::Running);
}

void ARyuMainCharacter1::TransitionRunningToWalking()
{
	OnRunningExit();
	State = ERyuMainCharacterStates::Walking;
	OnWalkingEnter();
}

void ARyuMainCharacter1::TransitionRunningToAttacking()
{
	OnRunningExit();
	State = ERyuMainCharacterStates::Attacking;
	OnAttackingEnter();
}

void ARyuMainCharacter1::TransitionRunningToDying()
{
	OnRunningExit();
	State = ERyuMainCharacterStates::Dying;
	OnDyingEnter();
}

void ARyuMainCharacter1::OnAttackingEnter()
{
	check(State == ERyuMainCharacterStates::Attacking);
}

void ARyuMainCharacter1::OnAttackingTick(float DeltaTime)
{
	check(State == ERyuMainCharacterStates::Attacking);
	TransitionAttackingToIdle();
}

void ARyuMainCharacter1::OnAttackingExit()
{
	check(State == ERyuMainCharacterStates::Attacking);
}

void ARyuMainCharacter1::TransitionAttackingToIdle()
{
	OnAttackingExit();
	State = ERyuMainCharacterStates::Idle;
	OnIdleEnter();
}

void ARyuMainCharacter1::OnDyingEnter()
{
	check(State == ERyuMainCharacterStates::Dying);
}

void ARyuMainCharacter1::OnDyingTick(float DeltaTime)
{
	check(State == ERyuMainCharacterStates::Dying);
	TransitionDyingToDead();
}

void ARyuMainCharacter1::OnDyingExit()
{
	check(State == ERyuMainCharacterStates::Dying);
}

void ARyuMainCharacter1::TransitionDyingToDead()
{
	OnDyingExit();
	State = ERyuMainCharacterStates::Dead;
	OnDeadEnter();
}

void ARyuMainCharacter1::OnDeadEnter()
{
	check(State == ERyuMainCharacterStates::Dead);
}

void ARyuMainCharacter1::OnDeadTick(float DeltaTime)
{
	check(State == ERyuMainCharacterStates::Dead);
}


*/
