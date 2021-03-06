// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuMovementComponent.h"
#include "RyuBaseCharacter.h"
#include "RyuMainCharacter.h"
#include <PaperFlipbookComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>

URyuMovementComponent::URyuMovementComponent()
{
}

URyuMovementComponent::URyuMovementComponent(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    //mostly just a Data Component
    VelocityAfterJumping.X = 0.0f;
    VelocityAfterJumping.Y = 450.0f;
    VelocityAfterJumping.Z = 0.0f;

    AfterJumpTreshold.X = 0.0f;
    AfterJumpTreshold.Y = 100.0f;
    AfterJumpTreshold.Z = -950.0f;

    JumpForce.X = 0.0f;
    JumpForce.Y = 300.0f;
    JumpForce.Z = 0.0f;

    JumpForceRun.X = 0.0f;
    JumpForceRun.Y = 0.0f;
    JumpForceRun.Z = 0.0f;

    AddFallingMultiplierNumber = 0.05f;
    GravityScaleMaximum = 4.0f;

    CoyoteTime = 0.3;

    CoyoteTimeActive = true;

    bDoStuffOnce = false;
}

void URyuMovementComponent::BeginPlay()
{
    Super::BeginPlay();
}

void URyuMovementComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

void URyuMovementComponent::JumpForward()
{
    if (auto* Owner = Cast<ARyuBaseCharacter>(GetOwner()))
    {
        FVector FinalJumpImpulse = JumpImpulse * Owner->GetActorForwardVector();
        UE_LOG(LogTemp, Log, TEXT("JumpForward: JumpImpulse = %s"), *FinalJumpImpulse.ToString());
        Owner->LaunchCharacter(FinalJumpImpulse, false, true);
    }
}

void URyuMovementComponent::JumpUp()
{
    if (auto* Owner = Cast<ARyuBaseCharacter>(GetOwner()))
    {
        FVector FinalJumpImpulse = JumpUpImpulse * Owner->GetActorUpVector();
        Owner->LaunchCharacter(FinalJumpImpulse, false, true);
    }
}

// TODO: BroadCastMovementMode with 2 Parameters ? -> PlayrMovement / ClimbingMode
void URyuMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode,
                                                  uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): Movement Mode changed ..."));

    ARyuMainCharacter* MainChar = Cast<ARyuMainCharacter>(CharacterOwner);

    /* ? */
    bDoStuffOnce = false;

    switch (MainChar->GetCharacterPossibility())
    {
        case ERyuCharacterPossibility::CanClimbLadderUp:
        {
            MainChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            MainChar->BoxTracer->SetEnableGravity(false);
            MainChar->GetSprite()->SetEnableGravity(false);
            //NoMesh in 2D Dude
            //MainChar->GetMesh()->SetEnableGravity(false);
            MainChar->GetCapsuleComponent()->SetEnableGravity(false);
            UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... to Custom::ClimbingLadder"));
            break;
        }
        default:
            break;
    }

    /*
    switch (CustomMovementMode)
    {
        case ERYUClimbingMode::CLIMBUPLEDGE:
        {
            //	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
            //UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): Colission %s"),*CapCol.ToString());
            //DONOT SET PARAMETER WHEN ABP ANIMATION ACTIVE !
            SetNoCollisionCharacterPrefs();
            MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
            MyChar->RYUClimbingMode = ERYUClimbingMode::CLIMBUPLEDGE;
            UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... to CLIMB- LEDGE"));
            OnMoveModeChanged.Broadcast(EPlayerMovement::CLIMBING);
            break;
        }
        case ERYUClimbingMode::CLIMBDOWNLEDGE:
        {
            //	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
            //UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
            //MyChar->FlipCharacter();
            //DONOT SET PARAMETER WHEN ABP ANIMATION ACTIVE !
            SetNoCollisionCharacterPrefs();
            ClimbDownStartPosition = MyChar->GetActorLocation();
            OnMoveModeChanged.Broadcast(EPlayerMovement::CLIMBING);
            MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
            MyChar->RYUClimbingMode = ERYUClimbingMode::CLIMBDOWNLEDGE;

            UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... to CLIMB-DOWN LEDGE"));

            break;
        }
        case ERYUClimbingMode::HANGONLEDGE:
        {
            UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... to Hanging: Waiting!"));

            OnMoveModeChanged.Broadcast(EPlayerMovement::CLIMBING);

            MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
            MyChar->RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;

            MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            MyChar->SphereTracer->SetEnableGravity(false);
            //MyChar->GetMesh()->SetEnableGravity(false);
            MyChar->GetCapsuleComponent()->SetEnableGravity(false);
            break;
        }
        case ERYUClimbingMode::JUMPTOLEDGE:
        {
            UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... to CustomJumpUp:"))
            OnMoveModeChanged.Broadcast(EPlayerMovement::CLIMBING);
            break;
        }

        default:
            break;
    }
    */

    /**FallBack , just use Flying */
    switch (MovementMode)
    {
        case MOVE_Flying:
        {
            //	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
            //UE_LOG(LogTemp, Log, TEXT("Col: %s"),*CapCol.ToString());
            MainChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            MainChar->BoxTracer->SetEnableGravity(false);
            MainChar->GetSprite()->SetEnableGravity(false);
            //NoMesh in 2D Dude
            //MainChar->GetMesh()->SetEnableGravity(false);
            MainChar->GetCapsuleComponent()->SetEnableGravity(false);
            UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... to FLYING"));
            break;
        }

        default:
            break;
    }
}

void URyuMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
    Super::PhysCustom(deltaTime, Iterations);

    if (deltaTime < MIN_TICK_TIME)
    {
        return;
    }

    switch (CustomMovementMode)
    {
        case ERyuCharacterState::ClimbUpLadder:
            if (!bDoStuffOnce)
            {
                bDoStuffOnce = true;
                UE_LOG(LogTemp, Log, TEXT("PhysCustom(): I�m climbing up !"));
            }
            PhysClimbingLadder(deltaTime, Iterations);

            break;

            /* TODO Cast to ne CharacterStateEnum*/
        case ERYUClimbingMode::CLIMBDOWNLEDGE:
            if (!bDoStuffOnce)
            {
                bDoStuffOnce = true;
                UE_LOG(LogTemp, Log, TEXT("PhysCustom(): I�m climbing down the ledge!"));
            }
            //PhysClimbingLedge(deltaTime, Iterations);
            break;
        case ERYUClimbingMode::HANGONLEDGE:
            //Just Hang around
            if (!bDoStuffOnce)
            {
                bDoStuffOnce = true;
                UE_LOG(LogTemp, Log, TEXT("PhysCustom(): Hanging!"));
            }
            break;
        case ERYUClimbingMode::FALLDOWNLEDGE:
            if (!bDoStuffOnce)
            {
                bDoStuffOnce = true;
                UE_LOG(LogTemp, Log, TEXT("PhysCustom(): I�m falling down the ledge!"));
            }
            PhysClimbingLedge(deltaTime, Iterations);
            //PhysFallingLedge(deltaTime, Iterations);
            break;
        case ERYUClimbingMode::JUMPTOLEDGE:
            if (!bDoStuffOnce)
            {
                bDoStuffOnce = true;
                UE_LOG(LogTemp, Log, TEXT("PhysCustom(): I�m Jumping to the ledge!"));
            }
    }
}

void URyuMovementComponent::PhysClimbingLedge(float deltaTime, int32 Iterations)
{
    //TODo: updates the movement take modified walking state ! ATM FlyingState active

    /** Following is copypasted from CharacterMovementComponent::PhysFlying and ajdusted to Phyclimbing*/

    //RestorePreAdditiveRootMotionVelocity();

    //ApplyRootMotionToVelocity(deltaTime);

    Iterations++;
    bJustTeleported = false;

    FVector OldLocation = UpdatedComponent->GetComponentLocation();
    const FVector Adjusted = Velocity * deltaTime;
    FHitResult Hit(1.f);
    SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

    if (Hit.Time < 1.f)
    {
        const FVector GravDir = FVector(0.f, 0.f, -1.f);
        const FVector VelDir = Velocity.GetSafeNormal();
        const float UpDown = GravDir | VelDir;

        bool bSteppedUp = false;
        if ((FMath::Abs(Hit.ImpactNormal.Z) < 0.2f) && (UpDown < 0.5f) && (UpDown > -0.2f)
            && CanStepUp(Hit))
        {
            float stepZ = UpdatedComponent->GetComponentLocation().Z;
            bSteppedUp = StepUp(GravDir, Adjusted * (1.f - Hit.Time), Hit);
            if (bSteppedUp)
            {
                OldLocation.Z = UpdatedComponent->GetComponentLocation().Z
                                + (OldLocation.Z - stepZ);
            }
        }

        if (!bSteppedUp)
        {
            //adjust and try again
            HandleImpact(Hit, deltaTime, Adjusted);
            SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
        }
    }

    if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
    {
        Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
    }
}

void URyuMovementComponent::PhysFallingLedge(float deltaTime, int32 Iterations)
{
}

void URyuMovementComponent::PhysClimbingLadder(float deltaTime, int32 Iterations)
{
    if (auto MainChar = Cast<ARyuMainCharacter>(GetOwner()))
    {
        if ((MainChar->GetCharacterState() != ERyuCharacterState::ExitLadder)
            && (MainChar->GetCharacterState() != ERyuCharacterState::EnterLadder))
        {
            auto CurrentPos = MainChar->GetActorLocation();
            FVector PosiUp = FVector(0.0f, 0.0f, MainChar->GetMoveUpInput());
            MainChar->SetActorLocation(CurrentPos + PosiUp);
        }
    }
}

void URyuMovementComponent::SetGravityScaleMaximum(float GravScale)
{
}

void URyuMovementComponent::SetJumpAllowedState(bool JumpState)
{
    MovementState.bCanJump = JumpState;
}

bool URyuMovementComponent::DoJump(bool bReplayingMoves)
{
    //Test JumpInput

    UE_LOG(LogTemp, Warning, TEXT("CharMovementDoJump(): JumpButton Pressed"));

    if (CharacterOwner && CharacterOwner->CanJump()
        || (GetOwner()->GetWorldTimerManager().GetTimerRemaining(Timerhandle_CoyoteTime) > 0.0f))
    {
        // Don't jump if we can't move up/down.
        if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
        {
            Velocity.Z = JumpZVelocity;
            SetMovementMode(MOVE_Falling);
            return true;
        }
    }

    return false;
}

// TODO in climbingcomp ?
void URyuMovementComponent::ResetClimbingState()
{
    if (auto MainChar = Cast<ARyuMainCharacter>(CharacterOwner))
    {
        //	ECollisionEnabled CapCol = MyChar->GetCapsuleComponent()->GetCollisionEnabled();
        //UE_LOG(LogTemp, Log, TEXT("ResetClimbingState() Col: %s"),*CapCol.ToString());
        UE_LOG(LogTemp, Log, TEXT("ResetClimbingState(): ClimbReset."));
        MainChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        //MainChar->SphereTracer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        //MainChar->SphereTracer->SetEnableGravity(true);
        MainChar->GetCapsuleComponent()->SetEnableGravity(true);

        //SetAllowClimbUpFalse();

        SetMovementMode(MOVE_Walking);

        ResetDoOnceClimbInput();

        //MainChar->CheckOverlappingActors();
    }
}

void URyuMovementComponent::SetNormalMaxJumpCount(int32 MaxJumps)
{
    NormalMaxJumpCount = MaxJumps;
}

void URyuMovementComponent::StartLaunchCharacter()
{
    // need pimp
    if (auto* Char = Cast<ARyuBaseCharacter>(GetOwner()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Launching Char when Jumping with %s"),
               *JumpImpulse.ToString());
        Char->LaunchCharacter(JumpImpulse, false, false);
    }
}

int32 URyuMovementComponent::GetNormalMaxJumpCount()
{
    return 0;
}

void URyuMovementComponent::ClimbDownLedgeFinished()
{
    ARYU2D_CharacterPrince* MyChar = Cast<ARYU2D_CharacterPrince>(CharacterOwner);
    MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
    MyChar->RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
    SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::HANGONLEDGE));
    SetAllowClimbUpTrue();
}

void URyuMovementComponent::SetNoCollisionCharacterPrefs()
{
    ARYU2D_CharacterBase* MyChar = Cast<ARYU2D_CharacterBase>(CharacterOwner);
    if (MyChar)
    {
        MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MyChar->SphereTracer->SetEnableGravity(false);
        MyChar->GetCapsuleComponent()->SetEnableGravity(false);
        //No need here --> managed in ABP
        MyChar->GetSprite()->SetLooping(false);
        MyChar->GetSprite()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

float URyuMovementComponent::GetSneakMultiplier()
{
    return SneakMultiplier;
}

FRyuMovementProperties URyuMovementComponent::GetRyuMovementState()
{
    return MovementState;
}

void URyuMovementComponent::IncreaseFallingVelocity()
{
    //TODO: when Jumping in JumpForwardFasterWay: increase VelocityZ even more !
    UE_LOG(LogRyu, Warning,
           TEXT("RyuMovementComponent(IncreaseFallingVelocity): Falling Speed(z): %f"),
           this->Velocity.Z);
    if ((Velocity.Z <= 0.0f) && (Velocity.Z >= MaxFallingVelocityZ))
    {
        Velocity.Z += (Velocity.Z * AddFallingMultiplierNumber);
    }
}

bool URyuMovementComponent::IsAllowedToJump()
{
    return MovementState.bCanJump;
}

void URyuMovementComponent::SetAllowClimbUpTrue()
{
    UE_LOG(LogTemp, Log, TEXT("SetAllowClimbUpTrue(): bAlloClimbing = true"));
    ARYU2D_CharacterPrince* MyChar = Cast<ARYU2D_CharacterPrince>(CharacterOwner);
    bClimbUpAllowed = true;
    MyChar->PlayerMovement = EPlayerMovement::CLIMBING;
    MyChar->RYUClimbingMode = ERYUClimbingMode::HANGONLEDGE;
}

void URyuMovementComponent::SetAllowClimbUpFalse()
{
    UE_LOG(LogTemp, Log, TEXT("SetAllowClimbUpTrue(): bAlloClimbing = false"));
    bClimbUpAllowed = false;
}

void URyuMovementComponent::SetVelocityAfterJump(FJumpStartValues AfterJumpValues)
{
    UE_LOG(LogRyu, Log, TEXT("SetVelocityAfterJump: %s Acc: %s"),
           *AfterJumpValues.JumpStartVelocity.ToString(),
           *AfterJumpValues.JumpStartAcceleration.ToString())
    //MaxWalkSpeed = AfterJumpValues.JumpStartVelocity.X;
    //MaxAcceleration = AfterJumpValues.JumpStartAcceleration.X;
    UE_LOG(LogRyu, Log, TEXT("VelocityMoveCompAfterJump: %s Acc: %s"), *Velocity.ToString(),
           *Acceleration.ToString())
}

void URyuMovementComponent::ResetDoOnceClimbInput()
{
    bDoOnceClimbInput = false;
}
