// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuMainCharacter.h"
#include "Components/RyuTimelineComponent.h"
#include "Components/RyuMovementComponent.h"
#include "Components/RyuDebugComponent.h"

#include "RYUClimbingActor.h"
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
	RyuTimelineComponent = CreateDefaultSubobject<URyuTimelineComponent>(TEXT("RyuTimeLineComponent"));
    

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
	LookDirection = ERyuLookDirection::Right;
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

	// TODO: replace with CSTM
    PlayerMovement = EPlayerMovement::STAND;
    CharAnimation2DState = ERYU2DAnimationState::IDLE;

    

    bSneakIsPressed = false;

    //*if not set
    //SneakMultiplier = 1.0f;

    SneakMultiplierValue = SneakMultiplier;
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

    //Temp save for Timelinestuff
    fDeltaSeconds = DeltaTime;

    

    //** due its better for complexicity AND clarity we do most of the ABP_Transition_Logic here in c++
    UpdateCharacter();
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
    //          bPressedJump = true;
    //          bJumpJustStarted = true;

    switch (PlayerMovement)
    {
        case EPlayerMovement::STAND:
        case EPlayerMovement::CANGRABLEDGE:

        {
            PlayerMovement = EPlayerMovement::JUMPSTART;
            break;
        }
    }
}

void ARyuMainCharacter::JumpForward()
{
    FVector FinalJumpImpulse = JumpImpulse * GetActorForwardVector();
    UE_LOG(LogTemp, Log, TEXT("JumpForward: JumpImpulse = %s"), *FinalJumpImpulse.ToString());
    LaunchCharacter(FinalJumpImpulse, false, true);
}

void ARyuMainCharacter::JumpUp()
{
    FVector FinalJumpImpulse = JumpUpImpulse * GetActorUpVector();
    LaunchCharacter(FinalJumpImpulse, false, true);
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
    if (bSneakIsPressed)
    {
        SneakMultiplierValue = SneakMultiplier;
    }
    else
    {
        SneakMultiplierValue = 1.0f;
    }

    MoveRightInput = Val * SneakMultiplierValue;

    /*
	if ((PlayerMovement != EPlayerMovement::BEGINRUN) &&
		(PlayerMovement != EPlayerMovement::ENDRUN) &&
		(PlayerMovement != EPlayerMovement::STARTTURN) &&
		(PlayerMovement != EPlayerMovement::ENDTURN))
		*/
    //TODO
    //** needs rework when turning whole running

    if (PlayerMovement == EPlayerMovement::STARTTURNRUN)
    {
        AddMovementInput(FVector(1.0f, 0.0f, 0.0f), -0.1f * Val);
        UE_LOG(LogTemp, Log, TEXT("MoveRight(): Turn while Running %s:"),
               *FString::SanitizeFloat((-0.1 * Val)));
    }

    if ((PlayerMovement == EPlayerMovement::STAND) || (PlayerMovement == EPlayerMovement::RUN)
        || (PlayerMovement == EPlayerMovement::CANGRABLEDGE)
        || (PlayerMovement == EPlayerMovement::WALK))

    {
        if ((LookDirection == ERyuLookDirection::Right && Val > 0) || (LookDirection == ERyuLookDirection::Left && Val < 0))
        {
            if (PlayerMovement == EPlayerMovement::CANGRABLEDGE)
                PlayerMovement = EPlayerMovement::BEGINRUN;
            AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveRightInput);
        }
    }
}

//Move Up or Down
void ARyuMainCharacter::MoveUp(float Value)
{
    //Class-global MoveUpAxis Input to use in several Methods inside the MainCharClass
    MoveUpInput = Value;
}

//* is called in MoveRight(Val), not in Tick anymore, but as MoveRight is bound to an axis this Ticks also
void ARyuMainCharacter::UpdateCharacter()
{
    currA = GetCharacterMovement()->GetCurrentAcceleration();
    currV = this->GetVelocity();

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
        if ((LookDirection == ERyuLookDirection::Right && MoveRightInput < 0) || (LookDirection == ERyuLookDirection::Left && MoveRightInput > 0))
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
            case EPlayerMovement::STAND: {
                if ((LookDirection == ERyuLookDirection::Right && (currV.X > 0)) || (LookDirection == ERyuLookDirection::Left && (currV.X < 0)))
                {
                    PlayerMovement = EPlayerMovement::BEGINRUN;
                    break;
                }
                //do we want to climb or JumpUp ?
                CheckMoveUpState();

                //return;
                break;
            }
            case EPlayerMovement::BEGINRUN: {
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
            case EPlayerMovement::RUN: {
                if (currV.X == 0)
                {
                    if (PlayerMovement != EPlayerMovement::STARTTURNRUN)
                        PlayerMovement = EPlayerMovement::ENDRUN;
                }

                //return;
                break;
            }
            case EPlayerMovement::ENDTURN: {
                PlayerMovement = EPlayerMovement::STAND;
                //return;
                break;
            }
            case EPlayerMovement::STARTFALLING: {
                PlayerMovement = EPlayerMovement::STANDUP;
                break;
            }
            case EPlayerMovement::FALLING: {
                PlayerMovement = EPlayerMovement::STANDUP;
                break;
            }
            case EPlayerMovement::CLIMBING: {
                Climb();
                break;
            }
            case EPlayerMovement::CANGRABLEDGE: {
                CheckMoveUpState();
                break;
            }
            default:
                break;
        }
    }
}

void ARyuMainCharacter::Climb()
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
                /* Without a Timeline but we need to adjust the Pivotpoint in EVERY Frame this SUCKS! maybe i can automatisize it when making my own animatins in PS
			(like we did in PS & Visionaire with a positioning exportfile?)*/
                MovementComp->SetMovementMode(MOVE_Custom,
                                              static_cast<uint8>(ERYUClimbingMode::CLIMBUPLEDGE));
                RYUClimbingMode = ERYUClimbingMode::CLIMBUPLEDGE;
            }
            break;
        default:
            break;
    }
}

void ARyuMainCharacter::SetClimbingPostitionsAndMovementMode(EPlayerMovement PlayerMove,
                                                             UBoxComponent* ClimbingTrigger)
{
    switch (PlayerMove)
    {
        case EPlayerMovement::JUMPUP: {
            //Use a Timeline for Positioning the Up-Jumps
            PlayerMovement = EPlayerMovement::JUMPUP;
            //** Initialize the Start End Endpoints
            Curve2DComponent->StartTimelineLocation = GetActorLocation();
            Curve2DComponent->EndTimelineLocation =
                FVector(Curve2DComponent->StartTimelineLocation.X,
                        Curve2DComponent->StartTimelineLocation.Y,
                        Curve2DComponent->StartTimelineLocation.Z
                            + Curve2DComponent->ClimbUpOffsetZ);
            MovementComp->SetMovementMode(MOVE_Custom,
                                          static_cast<uint8>(ERYUClimbingMode::JUMPTOLEDGE));
            SetCurrentTimelineParamsFloat(nullptr, Curve2DComponent->JumpUpFloatCurve, false, true);
            PlayTimeline();
            break;
        }

        case EPlayerMovement::JUMPSTART: {
            FVector CurrPlayerLocation = GetActorLocation();
            float CurJumpForwardDistance = 0;
            // TODO: find Direction of arrowcomponent (see climbupledgestates)
            //ArrowComponent->GetComponentLocation(). ? (CurJumpForwardDistance = MovementComp->JumpForwardDistance) : (CurJumpForwardDistance = MovementComp->JumpForwardDistance) *(-1);

            Curve2DComponent->StartTimelineLocation = CurrPlayerLocation;
            Curve2DComponent->EndTimelineLocation =
                FVector(CurrPlayerLocation.X, CurrPlayerLocation.Y, CurrPlayerLocation.Z);
        }

        case EPlayerMovement::CLIMBING: {
            UE_LOG(LogTemp, Log, TEXT("MoveUpState(Up): ClimbTriggerBox is %s : from Owner: %s "),
                   *ClimbingTrigger->GetName(), *ClimbingTrigger->GetOwner()->GetName());
            //TODO: search correct Actor, check if char needs to be flipped
            if ((PlayerMovement != EPlayerMovement::CLIMBING)
                && CheckFlipOverlappedActor(ClimbingTrigger))
            {
                FlipCharacter();
            }
            PlayerMovement = EPlayerMovement::CLIMBING;
            //sets the Movement of the player and approp. Specs
            UE_LOG(LogTemp, Log, TEXT("SetClimbingPostitionsAndMovementMode(): only once !"));
            SetClimbingPositions(ClimbingTrigger);
            //if (RYUClimbingMode == ERYUClimbingMode::CLIMBDOWNLEDGE)
            if (MoveUpInput < 0)
            {
                MovementComp->SetMovementMode(MOVE_Custom,
                                              static_cast<uint8>(ERYUClimbingMode::CLIMBDOWNLEDGE));
            }
            else
            {
                MovementComp->SetMovementMode(MOVE_Custom,
                                              static_cast<uint8>(ERYUClimbingMode::JUMPTOLEDGE));
            }

            //SetClimbingPositions(ClimbingTrigger);
        }
    }
}

//TODO: can be summarized, take a closer look ....
void ARyuMainCharacter::SetClimbingPositions(UBoxComponent* ClimbTrigger)
{
    ARYUClimbingActor* ARY = Cast<ARYUClimbingActor>(ClimbTrigger->GetOwner());

    if (ARY)
    {
        //TODO: better this LedPosiTag should be renamed to CurrentLedgePosiTrigger ! kindof confusing
        if (CurrentLedgePosiTagName == LeftLedgePosiTagName)
        {
            //@Relict?
            SetLedgeHangPosition(ARY->LeftHangPosition->GetComponentLocation(),
                                 LeftLedgePosiTagName);
            //NewPositioning
            //if character stands inside a trigger or climbs down a trigger
            //if UpAND DOwn the position needs to be set in Climb !
            //special case: canClimbUpAndDown
            if ((RYUClimbingMode == ERYUClimbingMode::CLIMBDOWNLEDGE)
                || (RYUClimbingMode == ERYUClimbingMode::CANCLIMBDOWNLEDGE)
                || ((RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN) && (MoveUpInput < 0)))
            {
                UE_LOG(LogTemp, Log, TEXT("SetClimbingPositions(): ClimbDown || CanClimbDown"));
                ClimbStandDownPosition = ARY->DownRightStandPosition->GetComponentLocation();
                ClimbStandUpPosition = ARY->UpLeftStandPosition->GetComponentLocation();
            }

            //if character stands inside a trigger or climbs up a trigger
            //if UpAND DOwn the position needs to be set in Climb !
            //special case: canClimbUpAndDown
            if ((RYUClimbingMode == ERYUClimbingMode::CLIMBUPLEDGE)
                || (RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPLEDGE)
                || ((RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN) && (MoveUpInput > 0)))
            {
                UE_LOG(LogTemp, Log, TEXT("SetClimbingPositions(): ClimbUp || CanClimbUp"));
                ClimbStandDownPosition = ARY->DownLeftStandPosition->GetComponentLocation();
                ClimbStandUpPosition = ARY->UpRightStandPosition->GetComponentLocation();
            }
        }

        //TODO: better this LedPosiTag should be renamed to CurrentLedgePosiTrigger ! kindof confusing
        if (CurrentLedgePosiTagName == RightLedgePosiTagName)
        {
            //@Relict?
            SetLedgeHangPosition(ARY->RightHangPosition->GetComponentLocation(),
                                 RightLedgePosiTagName);
            //NewPositioning

            //if character stands inside a trigger or climbs down a trigger
            //if UpAND DOwn the position needs to be set in Climb !
            //special case: canClimbUpAndDown
            if ((RYUClimbingMode == ERYUClimbingMode::CLIMBDOWNLEDGE)
                || (RYUClimbingMode == ERYUClimbingMode::CANCLIMBDOWNLEDGE)
                || ((RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN) && (MoveUpInput < 0)))
            {
                UE_LOG(LogTemp, Log, TEXT("SetClimbingPositions(): ClimbDown || CanClimbDown"));
                ClimbStandDownPosition = ARY->DownLeftStandPosition->GetComponentLocation();
                ClimbStandUpPosition = ARY->UpRightStandPosition->GetComponentLocation();
            }
            //if character stands inside a trigger or climbs up a trigger
            //if UpAND DOwn the position needs to be set in Climb !
            //special case: canClimbUpAndDown
            if ((RYUClimbingMode == ERYUClimbingMode::CLIMBUPLEDGE)
                || (RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPLEDGE)
                || ((RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN) && (MoveUpInput > 0)))
            {
                UE_LOG(LogTemp, Log, TEXT("SetClimbingPositions(): ClimbUp || CanClimbUp"));
                ClimbStandDownPosition = ARY->DownRightStandPosition->GetComponentLocation();
                ClimbStandUpPosition = ARY->UpLeftStandPosition->GetComponentLocation();
            }
        }

        UE_LOG(LogTemp, Log, TEXT("SetClimbingPositions(): ClimbStandDownPosi is %s"),
               *ClimbStandDownPosition.ToString());
        UE_LOG(LogTemp, Log, TEXT("SetClimbingPositions(): ClimbStandUp is %s"),
               *ClimbStandUpPosition.ToString());

        FVector PosChar = FVector(ClimbStandDownPosition.X, ClimbStandDownPosition.Y,
                                  ClimbStandDownPosition.Z + 50);
        SetActorLocation(PosChar);
    }
}

void ARyuMainCharacter::CheckMoveUpState()
{
    //TODO !!! and search correct climbingactior when in UpAndDownCLimbstate
    /*encapsulate stuff in functions */
    //Pressing Up at controller or keyboard
    if (MoveUpInput > 0)
    {
        //UE_LOG(LogTemp, Log, TEXT("CheckMoveUpState():"));
        switch (RYUClimbingMode)
        {
            case ERYUClimbingMode::NONE: {
                SetClimbingPostitionsAndMovementMode(EPlayerMovement::JUMPUP, nullptr);
                break;
            }

            case ERYUClimbingMode::CANCLIMBUPLEDGE: {
                if (UBoxComponent* ClimbTriggerBox =
                        GetOverlappedClimbingComponent(CanClimbUpTagName, CurrentLedgePosiTagName))
                {
                    SetClimbingPostitionsAndMovementMode(EPlayerMovement::CLIMBING,
                                                         ClimbTriggerBox);
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("No ClimbingTrigger found."));
                }
                break;
            }

            case ERYUClimbingMode::CANCLIMBUPANDDOWN: {
                if (UBoxComponent* ClimbTrigger =
                        GetOverlappedClimbingComponent(ERYULedgePosition2D::PosiUp))
                {
                    if (ARYUClimbingActor* ClimbingActor =
                            Cast<ARYUClimbingActor>(ClimbTrigger->GetOwner()))
                    {
                        CurrentLedgePosiTagName = ClimbTrigger->ComponentTags[1];
                        UE_LOG(LogTemp, Log, TEXT("MoveUpState(): Owner of Trigger is %s"),
                               *ClimbTrigger->GetOwner()->GetName());
                        SetClimbingPostitionsAndMovementMode(EPlayerMovement::CLIMBING,
                                                             ClimbTrigger);
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("No ClimbingTrigger found."));
                }

                break;
            }

            case ERYUClimbingMode::CANENTERLADDER:
                //TODO
                break;

            case ERYUClimbingMode::CLIMBUPLEDGE: {
                break;
            }

            default:
                break;
        }
    }
    //Pressing Down at controller or keyboard
    else if (MoveUpInput < 0)
    {
        switch (RYUClimbingMode)
        {
            case ERYUClimbingMode::NONE:
                //TODO: Crouch / Hock
                break;

            case ERYUClimbingMode::CANCLIMBDOWNLEDGE: {
                UBoxComponent* ClimbTriggerBox =
                    GetOverlappedClimbingComponent(CanClimbDownTagName, CurrentLedgePosiTagName);
                if (ClimbTriggerBox != nullptr)
                {
                    SetClimbingPostitionsAndMovementMode(EPlayerMovement::CLIMBING,
                                                         ClimbTriggerBox);
                    RYUClimbingMode = ERYUClimbingMode::CLIMBDOWNLEDGE;
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("No ClimbingTrigger found."));
                }
                break;
            }

            case ERYUClimbingMode::CANCLIMBUPANDDOWN: {
                UBoxComponent* ClimbTrigger =
                    GetOverlappedClimbingComponent(ERYULedgePosition2D::PosiDown);
                if (ClimbTrigger != nullptr)
                {
                    ARYUClimbingActor* ClAc = Cast<ARYUClimbingActor>(ClimbTrigger->GetOwner());
                    if (ClAc)
                    {
                        UE_LOG(LogTemp, Log, TEXT("MoveUpState(): Owner of Trigger is %s"),
                               *ClimbTrigger->GetOwner()->GetName());
                        CurrentLedgePosiTagName = ClimbTrigger->ComponentTags[1];
                        // 						SetActorLocation(ClAc->DownLeftStandPosition->GetComponentLocation());
                        // 						if (CheckFlipOverlappedActor(ClimbTrigger))
                        // 						{
                        // 							FlipCharacter();
                        // 						}
                        // 						MovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::CLIMBDOWNLEDGE));
                        SetClimbingPostitionsAndMovementMode(EPlayerMovement::CLIMBING,
                                                             ClimbTrigger);
                        RYUClimbingMode = ERYUClimbingMode::CLIMBDOWNLEDGE;
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("No ClimbingTrigger found."));
                }
                break;
            }

            default:
                break;
        }
    }
}

UBoxComponent* ARyuMainCharacter::GetOverlappedClimbingComponent(ERYULedgePosition2D LedgePosi)
{
    //make sure only relevant BoxTrigger are in the Componentsarray
    GetOverlappingBoxComponents();

    //sort Array resp. Componentlocation
    CapsuleOverlappedComponents.Sort([LedgePosi](auto& lhs, auto& rhs) {
        if (LedgePosi == ERYULedgePosition2D::PosiUp)
        {
            return ((lhs.GetOwner()->GetActorLocation().Z)
                    > (rhs.GetOwner()->GetActorLocation().Z));
        }
        if (LedgePosi == ERYULedgePosition2D::PosiDown)
        {
            return ((lhs.GetOwner()->GetActorLocation().Z)
                    < (rhs.GetOwner()->GetActorLocation().Z));
        }
        else
            return false;
    });

    OutputCapsuleOverlappedComponents();

    return CapsuleOverlappedComponents[0] ? Cast<UBoxComponent>(CapsuleOverlappedComponents[0])
                                          : nullptr;
}

UBoxComponent* ARyuMainCharacter::GetOverlappedClimbingComponent(FName UpOrDown, FName LeftOrRight)
{
    UE_LOG(LogTemp, Log, TEXT("GetOverlappedClimbComp(): %s , %s"), *UpOrDown.ToString(),
           *LeftOrRight.ToString());

    FName ClimbUpDownCheck;

    if (RYUClimbingMode == ERYUClimbingMode::CANCLIMBDOWNLEDGE)
    {
        ClimbUpDownCheck = CanClimbDownTagName;
    }
    if (RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPLEDGE)
    {
        ClimbUpDownCheck = CanClimbUpTagName;
    }

    UE_LOG(LogTemp, Log, TEXT("GetOverlappedClimbComp(): ClimbCheck: %s"),
           *ClimbUpDownCheck.ToString());

    // uses Lambda-functionality / use of Algorithm (FindByPredicate); LERN DIE ALGORITHMEN ALDER SONST HANDKANTE ! (orig. MaxZitat XD )
    //Max:
    UPrimitiveComponent** Foo = nullptr;
    if (ClimbUpDownCheck == UpOrDown)
    {
        Foo = CapsuleOverlappedComponents.FindByPredicate([&UpOrDown, &LeftOrRight](auto* x) {
            auto* OvCC = Cast<UBoxComponent>(x);
            return OvCC && (OvCC->ComponentTags[0] == UpOrDown)
                   && (OvCC->ComponentTags[1] == LeftOrRight);
        });
    }

    return Foo ? Cast<UBoxComponent>(*Foo) : nullptr;
}

void ARyuMainCharacter::StartLaunchCharacter()
{
    // need pimp
    UE_LOG(LogTemp, Warning, TEXT("Launching Char when Jumping with %s"), *JumpImpulse.ToString());
    LaunchCharacter(JumpImpulse, false, false);
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

void ARyuMainCharacter::TimelineCallbackFloat(float val)
{
    // This function is called for every tick when the timeline runs.
    //UE_LOG(LogTemp, Log, TEXT("TimelineCallbackFloat(): Here incr. z-Location: %s"), *FString::SanitizeFloat(val));

    FVector StartTLLocation;
    FVector EndTLLocation;

    StartTLLocation = Curve2DComponent->StartTimelineLocation;
    EndTLLocation = Curve2DComponent->EndTimelineLocation;

    //Switch PMove needable ? ? ? or are all timelines handled the same ?
    switch (PlayerMovement)
    {
        case EPlayerMovement::JUMPUP:
            //TODO: look for ClimbUp vs. Down etc // see mix from 3D LedgeClimbing but wo Rootmotion ...

            break;
        default:
            break;
    }

    SetActorLocation(FMath::Lerp(StartTLLocation, EndTLLocation, val));
    //AddActorLocalOffset(FVector(0,0,CurrentTimeline->));
}

void ARyuMainCharacter::TimelineCallbackVector(FVector Vec)
{
    // This function is called for every tick in the timeline.
    //UE_LOG(LogTemp, Log, TEXT("TimelineCallbackVector(): Here incr. z-Location: %s"), *FString::SanitizeFloat(val));

    FVector StartTLLocation;
    FVector EndTLLocation;

    StartTLLocation = Curve2DComponent->StartTimelineLocation;
    EndTLLocation = Curve2DComponent->EndTimelineLocation;

    //Switch PMove needable ? ? ? or are all timelines handled the same ?
    switch (PlayerMovement)
    {
        case EPlayerMovement::JUMPUP:
            //TODO: look for ClimbUp vs. Down etc // see mix from 3D LedgeClimbing but wo Rootmotion ...

            break;
        default:
            break;
    }

    SetActorLocation(FMath::Lerp(StartTLLocation, EndTLLocation, Vec));
    //AddActorLocalOffset(FVector(0,0,CurrentTimeline->));
}

void ARyuMainCharacter::TimelineFinishedCallback()
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

void ARyuMainCharacter::PlayTimeline()
{
    if (CurrentTimeline != NULL)
    {
        UE_LOG(LogTemp, Log, TEXT("PlayTimeline(): Play a timeline from Start"));
        CurrentTimeline->PlayFromStart();
    }
}

void ARyuMainCharacter::SetCurrentTimelineParamsFloat(UCurveFloat* FloatCurveX,
                                                      UCurveFloat* FloatCurveZ,
                                                      bool TimelineIsLooping,
                                                      bool IgnoreTimeDilation)
{
    if (FloatCurveX)
    {
        UE_LOG(LogTemp, Log,
               TEXT("SetCurrentTimelineParamsFloat(): Set TimeLine Params for Timeline X %s"),
               *FloatCurveX->GetName());
        CurrentTimeline->AddInterpFloat(FloatCurveX, onTimelineCallbackFloat, FName("MovementX"));
    }

    if (FloatCurveZ)
    {
        UE_LOG(LogTemp, Log,
               TEXT("SetCurrentTimelineParamsFloat(): Set TimeLine Params for Timeline Z %s"),
               *FloatCurveZ->GetName());
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

void ARyuMainCharacter::SetCurrentTimelineParamsVector(UCurveVector* VectorCurve,
                                                       bool TimelineIsLooping,
                                                       bool IgnoreTimeDilation)
{
    if (VectorCurve)
    {
        UE_LOG(LogTemp, Log,
               TEXT("SetCurrentTimelineParamsVector(): Set TimeLine Params for Timeline %s"),
               *VectorCurve->GetName());
        //Add Float curve to the timeline and bind it to the interpfunction´s delegate
        //3rd Parameter = floatValue, Propertyname, Bind all Stuff
        CurrentTimeline->AddInterpVector(VectorCurve, onTimelineCallbackVector,
                                         FName("MovementVec"));
        CurrentTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

        // Timeline Settings
        CurrentTimeline->SetLooping(TimelineIsLooping);
        //false = it moves as the global Time Dileation
        CurrentTimeline->SetIgnoreTimeDilation(IgnoreTimeDilation);
    }
}

void ARyuMainCharacter::HangOnLedgeAndClimb(float Val)
{
}

void ARyuMainCharacter::CanGrabLedges(float Val)
{
}

void ARyuMainCharacter::JumpUpOrDown(float Val, FVector StartJumpPosition)
{
}

/** Getter and Setter*/

FVector ARyuMainCharacter::GetCurrentVelocity()
{
    return currV;
}

FVector ARyuMainCharacter::GetCurrentAcceleration()
{
    return currA;
}

ERyuLookDirection ARyuMainCharacter::GetLookDirection()
{
    //return bLookRight;
	return LookDirection;
}

void ARyuMainCharacter::SetLookRight()
{
    if (currV.X > 0)
    {
		LookDirection == ERyuLookDirection::Right;
        //bLookRight = true;
    }
    else if (currV.X < 0)
    {
		LookDirection == ERyuLookDirection::Left;
        //bLookRight = false;
    }
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

void ARyuMainCharacter::ClimbLedgeFlipBookFinished()
{
    UE_LOG(LogTemp, Log,
           TEXT("ClimbLedgeFlipBookFinished(): Call From Notify->ClimbUpFlippbookFinished"));

    switch (RYUClimbingMode)
    {
        case ERYUClimbingMode::CLIMBDOWNLEDGE: {
            RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
        }
        break;
        case ERYUClimbingMode::CLIMBUPLEDGE: {
            SetActorLocation(ClimbStandUpPosition);
            ResetClimbingState();
            break;
        }
        case ERYUClimbingMode::LETGOLEDGE: {
            UE_LOG(LogTemp, Log, TEXT("ClimbLedgeFlipBookFinished(): LETGOEND"));
            ResetClimbingState();
            break;
        }

        default:
            break;
    }
}

void ARyuMainCharacter::ResetClimbingState()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereTracer->SetEnableGravity(true);
    GetCapsuleComponent()->SetEnableGravity(true);
    MovementComp->SetMovementMode(MOVE_Walking);
    CheckOverlappingActors();
}

void ARyuMainCharacter::AnimationSequenceEnded(const UPaperZDAnimSequence* InAnimSequence)
{
    UE_LOG(LogTemp, Warning, TEXT("Finally AnimEnded called from code."));

    /* Just for testing purpose until the StateMachine is fully implemented ! -> for removing finally the Notifier in ABP*/
    switch (PlayerMovement)
    {
        case EPlayerMovement::STAND:
            break;
        case EPlayerMovement::WALK:
            break;
        case EPlayerMovement::STARTTURN:
            TurnFlipBookFinished();
            break;
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
            CheckOverlappingActors();
            break;
        case EPlayerMovement::JUMPSTART: {
            PlayerMovement = EPlayerMovement::JUMPLOOP;
            JumpForward();
        }
        break;
        case EPlayerMovement::JUMPLOOP:
            break;
        case EPlayerMovement::JUMPEND:
            CheckOverlappingActors();
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
                    SetClimbingMode(ERYUClimbingMode::HANGONLEDGE);
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

bool ARyuMainCharacter::CheckFlipOverlappedActor(UBoxComponent* ClimbingTrigger)
{
    //TODO: search correct actor

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

void ARyuMainCharacter::SneakPressed()
{
    if (PlayerMovement != EPlayerMovement::CLIMBING)
    {
        SneakMultiplierValue = SneakMultiplier;
        bSneakIsPressed = true;
    }
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

void ARyuMainCharacter::ChangeMovementMode()
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

void ARyuMainCharacter::ChangeClimbingMode()
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
