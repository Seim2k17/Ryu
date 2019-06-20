// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuMainCharacter.h"
#include "Components/RyuClimbingComponent.h"
#include "Components/RyuDebugComponent.h"
#include "Components/RyuMovementComponent.h"
#include "Components/RyuTimelineComponent.h"
#include "Enums/ERyuInputState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYUClimbingActor.h"
#include "RYU_prototype.h"
#include "RyuCharacterState.h"
#include <PaperFlipbookComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <Curves/CurveVector.h>
#include <GameFramework/Character.h>
#include <GameFramework/SpringArmComponent.h>
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
}
#endif

// TODO CSTM relevant
void ARyuMainCharacter::AnimationSequenceEnded(const UPaperZDAnimSequence* InAnimSequence)
{
    UE_LOG(LogTemp, Warning, TEXT("Finally AnimEnded called from code."));

    ERYUClimbingMode RYUClimbingMode = RyuClimbingComponent->GetClimbingState();

    /* Just for testing purpose until the StateMachine is fully implemented ! -> for removing finally the Notifier in ABP*/
    switch (PlayerMovement)
    {
        case EPlayerMovement::STAND:
            break;
        case EPlayerMovement::WALK:
            break;
        // TODO: is this even needable, because we큞l integrate the start/end-Anis in the Transitions of the ABP! -> CHECK
        case EPlayerMovement::STARTTURN:
            TurnFlipBookFinished();
            break;
        // TODO: is this even needable, because we큞l integrate the start/end-Anis in the Transitions of the ABP! -> CHECK
        case EPlayerMovement::STARTTURNRUN:
            TurnRunFlipBookFinished();
            break;
        case EPlayerMovement::ENDTURN:
            break;
        case EPlayerMovement::BEGINRUN:
            PlayerMovement = EPlayerMovement::RUN;
            break;
        case EPlayerMovement::RUN:
            break;
        case EPlayerMovement::ENDRUN:
            CheckOverlapClimbableActors();
            break;
        case EPlayerMovement::JUMPSTART:
        {
            PlayerMovement = EPlayerMovement::JUMPLOOP;
            RyuMovementComponent->JumpForward();
        }
        break;
        case EPlayerMovement::JUMPLOOP:
            break;
        case EPlayerMovement::JUMPEND:
            CheckOverlapClimbableActors();
            break;
        case EPlayerMovement::JUMPUP:
            break;
        case EPlayerMovement::STARTFALLING:
            break;
        case EPlayerMovement::FALLING:
            break;
        case EPlayerMovement::CLIMBING:
            switch (RYUClimbingMode)
            {
                case ERYUClimbingMode::NONE:
                    break;
                case ERYUClimbingMode::CANCLIMBUPLEDGE:
                case ERYUClimbingMode::CANCLIMBDOWNLEDGE:
                case ERYUClimbingMode::CANCLIMBUPANDDOWN:
                case ERYUClimbingMode::CLIMBDOWNLEDGE:
                    RyuClimbingComponent->SetClimbingState(ERYUClimbingMode::HANGONLEDGE);
                    break;
                case ERYUClimbingMode::JUMPTOLEDGE:
                    break;
                case ERYUClimbingMode::CLIMBUPLEDGE:
                case ERYUClimbingMode::LETGOLEDGE:
                    ClimbLedgeFlipBookFinished();
                    break;
                case ERYUClimbingMode::FALLDOWNLEDGE:
                    break;
                case ERYUClimbingMode::HANGONLEDGE:
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
            break;
        case EPlayerMovement::CANGRABLEDGE:
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

    bSneakIsPressed = false;
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
}

void ARyuMainCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //TODO implement it correct then call it / atm crash -> pure virtual function call !
    CharacterState->Update(this);

    //Temp save for Timelinestuff
    fDeltaSeconds = DeltaTime;

    //** due its better for complexicity AND clarity we do most of the ABP_Transition_Logic here in c++
    // TODO: integrate this stuff in the new CharacterStateMachine !

    //UpdateCharacter();
}

void ARyuMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAxis("MoveRight", this, &ARyuMainCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Climb", this, &ARyuMainCharacter::MoveUp);
    PlayerInputComponent->BindAction("Sneak", IE_Pressed, this, &ARyuMainCharacter::SneakPressed);
    PlayerInputComponent->BindAction("Sneak", IE_Released, this, &ARyuMainCharacter::SneakReleased);
}

void ARyuMainCharacter::Jump()
{
    // TODO: is this even executed atm ? see SetupPlayerInputComponent
    // from CharacterClass
    //          bPressedJump = true;
    //          bJumpJustStarted = true;
    HandleInput(ERyuInputState::PressJump);
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

    Super::ResetJumpState();
}

//Move Left or Right
void ARyuMainCharacter::MoveRight(float Val)
{
    //for slow Movement , TODO: Clamp for ControllerInput (AxisValus...)

    bool PressRight = false;

    float SneakValue = 1.0f;

    bSneakIsPressed ? SneakValue = RyuMovementComponent->GetSneakMultiplier() : SneakValue = 1.0f;

    MoveRightInput = Val * SneakValue;

    if (bSneakIsPressed)
    {
        //HandleInput(ERyuInputState::PressSneak);
    }

    // Bind to Press / Release Button Events ?
    if (MoveRightInput < 0)
    {
        PressRight = false;
        HandleInput(ERyuInputState::PressLeft);
    }
    else if (MoveRightInput > 0)
    {
        PressRight = true;
        HandleInput(ERyuInputState::PressRight);
    }
    else
    {
        if (PressRight)
        {
            HandleInput(ERyuInputState::ReleaseRight);
        }
        else
        {
            HandleInput(ERyuInputState::ReleaseLeft);
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

    if (PlayerMovement == EPlayerMovement::JUMPLOOP && !bIsinAir)
    {
        PlayerMovement = EPlayerMovement::JUMPEND;
    }

    //** ABP-TRANSITION-RULES *******//
    //** TransitionRules for the ABP, moved it completely to c++ due clarity and complexicity reason
    if (currV.Z < -500)
    {
        if (PlayerMovement != EPlayerMovement::FALLING)
        {
            PlayerMovement = EPlayerMovement::STARTFALLING;
        }
    }
    else
    {
        if (bSneakIsPressed)
        {
            PlayerMovement = EPlayerMovement::SNEAK;
        }

        //TODO curV.Z > 0 (was auch immer das heisst)

        //Character can Turn around --> it playes the turnAnimation
        if ((GetLookDirection() == ERyuLookDirection::Right && MoveRightInput < 0)
            || (GetLookDirection() == ERyuLookDirection::Left && MoveRightInput > 0))
        {
            if (PlayerMovement == EPlayerMovement::RUN)
            {
                PlayerMovement = EPlayerMovement::STARTTURNRUN;
                UE_LOG(LogTemp, Log, TEXT("UpdateCharacter(): Started Turn while Running"));
            }
            else
            {
                if ((PlayerMovement == EPlayerMovement::STAND)
                    || (PlayerMovement == EPlayerMovement::CANGRABLEDGE))
                    PlayerMovement = EPlayerMovement::STARTTURN;
            }
        }

        //character IS Turning
        if (PlayerMovement == EPlayerMovement::STARTTURNRUN)
            return;

        switch (PlayerMovement)
        {
            case EPlayerMovement::STAND:
            {
                if ((GetLookDirection() == ERyuLookDirection::Right && (currV.X > 0))
                    || (GetLookDirection() == ERyuLookDirection::Left && (currV.X < 0)))
                {
                    PlayerMovement = EPlayerMovement::BEGINRUN;
                    break;
                }
                //do we want to climb or JumpUp ?
                RyuClimbingComponent->CheckMoveUpState(MoveUpInput);

                //return;
                break;
            }
            case EPlayerMovement::BEGINRUN:
            {
                //change State to stop walking or Running
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
            }
            case EPlayerMovement::RUN:
            {
                if (currV.X == 0)
                {
                    if (PlayerMovement != EPlayerMovement::STARTTURNRUN)
                        PlayerMovement = EPlayerMovement::ENDRUN;
                }

                //return;
                break;
            }
            case EPlayerMovement::ENDTURN:
            {
                PlayerMovement = EPlayerMovement::STAND;
                //return;
                break;
            }
            case EPlayerMovement::STARTFALLING:
            {
                PlayerMovement = EPlayerMovement::STANDUP;
                break;
            }
            case EPlayerMovement::FALLING:
            {
                PlayerMovement = EPlayerMovement::STANDUP;
                break;
            }
            case EPlayerMovement::CLIMBING:
            {
                RyuClimbingComponent->Climb(MoveUpInput);
                break;
            }
            case EPlayerMovement::CANGRABLEDGE:
            {
                RyuClimbingComponent->CheckMoveUpState(MoveUpInput);
                break;
            }
            default:
                break;
        }
    }
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

void ARyuMainCharacter::HandleInput(ERyuInputState Input)
{
    // TODO: check if objects are created every frame !? /// HM DAMN it crashes sometimes ?
    IRyuCharacterState* state = CharacterState->HandleInput(this, Input);
    UE_LOG(LogRyu, Warning, TEXT("GetCharState: %s"),
           *URyuStaticFunctionLibrary::GetCharacterStateName(CharacterState->GetState()));
    if (state != nullptr)
    {
        // Call Exit-Action on the old state
        CharacterState->Exit(this);
        EquipmentState->Exit(this);

        // delete old CharacterState;
		
        CharacterState = state;
        EquipmentState = state;

        // Call the enter Action on the new State
        CharacterState->Enter(this);
        EquipmentState->Enter(this);
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

void ARyuMainCharacter::TurnFlipBookFinished()
{
    FlipCharacter();
}

void ARyuMainCharacter::TurnRunFlipBookFinished()
{
    FlipCharacter();

    PlayerMovement = EPlayerMovement::RUN;
}

bool ARyuMainCharacter::CheckFlipOverlappedActor(UBoxComponent* ClimbingTrigger)
{
    //TODO: search correct actor, do we need the ArrowComponent ? guess it큦 only for EditorPurposes

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
                       TEXT("CheckFlipOverlappedActor(): Yes it큦 a climbing trigger."));
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

bool ARyuMainCharacter::GetSneakActive()
{
    return bSneakIsPressed;
}

void ARyuMainCharacter::SneakPressed()
{
    SneakMultiplierValue = RyuMovementComponent->GetSneakMultiplier();
    bSneakIsPressed = true;
}

void ARyuMainCharacter::SneakReleased()
{
    SneakMultiplierValue = 1.0f;
    bSneakIsPressed = false;
}

void ARyuMainCharacter::ConfigurePlayer_Implementation(UPaperZDAnimPlayer* Player)
{
    Super::ConfigurePlayer_Implementation(Player);

    UPaperZDAnimInstance* AnimInstance = GetOrCreateAnimInstance();

    Player->OnPlaybackSequenceComplete.AddDynamic(this, &ARyuMainCharacter::AnimationSequenceEnded);
}

/** CHaracter State Machine Prep 

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
