// Fill out your copyright notice in the Description page of Project Settings.

#include "RyuBaseCharacter.h"
#include "Components/RyuClimbingComponent.h"
#include "Components/RyuDebugComponent.h"
#include "Components/RyuMovementComponent.h"
#include "Enums/ERyuButtonType.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "IO/RyuAbilityCommand.h"
#include "IO/RyuBlockCommand.h"
#include "IO/RyuDuckCommand.h"
#include "IO/RyuHighStrikeCommand.h"
#include "IO/RyuInteractCommand.h"
#include "IO/RyuItemSelectCommand.h"
#include "IO/RyuJumpCommand.h"
#include "IO/RyuLowStrikeCommand.h"
#include "IO/RyuMoveRightCommand.h"
#include "IO/RyuShardSelectCommand.h"
#include "IO/RyuSneakCommand.h"
#include "IO/RyuSprintCommand.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "ERyuLookDirection.h"
#include "RYU2DENUM_Movement.h"
#include "RYUClimbingActor.h"
#include "RYU_prototype.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterJumpState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterState.h"
#include <Camera/CameraComponent.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Math/Vector.h>
#include <PaperZD/Public/AnimSequences/Players/PaperZDAnimPlayer.h>
#include <PaperZD/Public/PaperZDAnimBP.h>
#include <PaperZD/Public/PaperZDAnimInstance.h>

ARyuBaseCharacter::ARyuBaseCharacter()
{
}

ARyuBaseCharacter::ARyuBaseCharacter(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<URyuMovementComponent>(
        ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;
    //PrimaryActorTick.TickInterval = 0.05f;

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
    SphereTracer->SetSphereRadius(60);

    bSphereTracerOverlap = false;

    bJumpJustStarted = false;

    TreshholdYWalkRun = 220.0f;

    // Create CharacterState
    // PlayerMovement = EPlayerMovement::STAND;
    CharacterState = NewObject<URyuCharacterIdleState>();
    EquipmentState = NewObject<URyuCharacterIdleState>();

    InitializeCommands();
}

void ARyuBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if ((CharacterState != nullptr) //&& (CheckCharacterEnumValue())
        && (CharacterState->GetInputPressedState() != ERyuInputState::None))
    {
        CharacterState->Update(this);
    }

    // UE_LOG(LogRyu, Log, TEXT("TickIntervall@BaseChar: %f"), this->PrimaryActorTick.TickInterval);
}

void ARyuBaseCharacter::AddCurrentInputState(ERyuInputState InputStateToAdd)
{
    CurrentInputs.Add(InputStateToAdd);
}

void ARyuBaseCharacter::CheckCurrentInputState(
    ERyuInputState InputState, ERyuButtonType ButtonType /* = ERyuButtonType::None */)
{
    if (URyuStaticFunctionLibrary::InputStateToString(InputState).Contains(TEXT("Press")))
    {
        AddCurrentInputState(InputState);
    }
    // ReleaseButton
    else if (URyuStaticFunctionLibrary::InputStateToString(InputState).Contains(TEXT("Release")))
    {
        if (KeyInputCounterpartMap[InputState].ButtonType == ERyuButtonType::Axis)
        {
            UE_LOG(LogRyu, Warning, TEXT("InpuState: %s"),
                   *URyuStaticFunctionLibrary::InputStateToString(InputState));
            PressedState = KeyInputCounterpartMap.Find(InputState);
            RemoveInputeState(PressedState->PressKeyState);
        }

        //         if (KeyInputCounterpartMap.Contains(InputState)
        //             && KeyInputCounterpartMap[InputState].ButtonType
        //                    == ERyuButtonType::Axis /* ButtonType */)
        //         {
        //             RemoveInputeState(InputState);
        //         }
    }
}

void ARyuBaseCharacter::RemoveInputeState(ERyuInputState InputStateToRemove)
{
    CurrentInputs.Remove(InputStateToRemove);
}

void ARyuBaseCharacter::ResetMoveRightInput()
{
    MoveRightAxisState = ERyuMoveRightAxisInputState::Inactive;
}

void ARyuBaseCharacter::ResetEndJumpTimer()
{
    // pass in link to Character from Timer to reset State !!!
    UE_LOG(LogRyu, Log, TEXT("RyuBaseCharacter: TimerResetJumping."));

    HandleInput(ERyuInputState::InputEndJump);
    GetWorld()->GetTimerManager().ClearTimer(EndJumpTimerHandle);
}

void ARyuBaseCharacter::ResetFallingTimer()
{
    UE_LOG(LogRyu, Log, TEXT("RyuBaseCharacter: TimerResetFalling."));
    GetWorld()->GetTimerManager().ClearTimer(FallingTimerHandle);
}

void ARyuBaseCharacter::ResetJumpStartValues()
{
    JumpStartValues.JumpStartAcceleration = FVector::ZeroVector;
    JumpStartValues.JumpStartVelocity = FVector::ZeroVector;
}

void ARyuBaseCharacter::SaveJumpValues()
{
    UE_LOG(LogRyu, Log, TEXT("BaseCharacter: SaveJumpValues."));
    JumpStartValues.JumpStartVelocity = GetCharacterMovement()->Velocity;
    JumpStartValues.JumpStartAcceleration = GetCharacterMovement()->GetCurrentAcceleration();
}

void ARyuBaseCharacter::AnimationSequenceEnded(const UPaperZDAnimSequence* InAnimSequence)
{
    UE_LOG(LogTemp, Warning, TEXT("Finally AnimEnded called from code."));
    // Reactions to AnimationEnded are handled in the appr. States
    HandleInput(ERyuInputState::AnimationEnded);
}

void ARyuBaseCharacter::OnSphereTracerHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                         AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp,
                                                         int32 OtherBodyIndex, bool bFromSweep,
                                                         const FHitResult& SweepResult)
{
    //OnSphereTracerCheckOverlap(OtherActor, OtherComp);
}

void ARyuBaseCharacter::OnSphereTracerHandleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                       AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp,
                                                       int32 OtherBodyIndex)
{
    // 	bSphereTracerOverlap = false;
    // 	bLedgeHeightInRange = false;
    // 	SphereOverlappedActor = nullptr;
    // 	UE_LOG(LogTemp, Log, TEXT("OnSphereTracerHandleEndOverlap(): SpherTracer Overlap Out"));
    // 	if ((RYUClimbingMode != ERYUClimbingMode::CANCLIMBDOWNLEDGE) &&
    // 		(RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPANDDOWN) &&
    // 		(RYUClimbingMode != ERYUClimbingMode::CANCLIMBUPLEDGE) &&
    // 		(RYUClimbingMode != ERYUClimbingMode::CLIMBDOWNLEDGE) &&
    // 		(RYUClimbingMode != ERYUClimbingMode::CLIMBUPLEDGE) &&
    // 		(PlayerMovement != EPlayerMovement::CLIMBING))
    // 	{
    // 		UE_LOG(LogTemp, Log, TEXT("OnSphereTracerHandleEndOverlap(): SphereTracer->Mode->Walk Activated"));
    // 		PlayerMovement = EPlayerMovement::WALK;
    // 	}
}

void ARyuBaseCharacter::OnSphereTracerCheckOverlap(AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp)
{
    ARYUClimbingActor* IgnoredActor_1 = Cast<ARYUClimbingActor>(OtherActor);
    if (OtherActor != nullptr)
        UE_LOG(LogTemp, Log, TEXT("OnSphereTracerCheckOverlap(): SphereTracer Overlap In with: %s"),
               *OtherActor->GetName());

    if ((OtherActor != nullptr) &&
        //(!ClimbComponentOverlap) &&
        (!IgnoredActor_1))
    {
        //double Set when calling from OnHandleCapsuleEndOverlap, But WHO CARES ?
        SphereOverlappedActor = OtherActor;
        SphereOverlappedComponent = OtherComp;

        bSphereTracerOverlap = true;

        /*

        // TODO to ClimbingStateMachine
		ERYUClimbingMode ClimbState = RyuClimbingComponent->GetClimbingState();

        if ((ClimbState != ERYUClimbingMode::CANCLIMBDOWNLEDGE)
            && (ClimbState != ERYUClimbingMode::CANCLIMBUPANDDOWN)
            && (ClimbState != ERYUClimbingMode::CANCLIMBUPLEDGE)
            && (ClimbState != ERYUClimbingMode::CLIMBDOWNLEDGE)
            && (ClimbState != ERYUClimbingMode::CLIMBUPLEDGE)
            //&& (PlayerMovement != EPlayerMovement::CLIMBING))

        {
            UE_LOG(LogTemp, Log,
                   TEXT("OnSphereTracerCheckOverlap(): SphereTracer->Mode->CanTraceActivated"));
            //PlayerMovement = EPlayerMovementMode::CANTRACELEDGE;
            //PlayerMovement = EPlayerMovement::CANGRABLEDGE;
        }

		*/
    }
}

void ARyuBaseCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    RyuMovementComponent = Cast<URyuMovementComponent>(GetCharacterMovement());
}

void ARyuBaseCharacter::AllowReleaseKey()
{
    SetAllowReleaseAxisKey(true);
}

void ARyuBaseCharacter::IncreaseFallingVelocity()
{
    if (auto* MoveComp = Cast<URyuMovementComponent>(GetCharacterMovement()))
    {
        MoveComp->IncreaseFallingVelocity();
    }
}

void ARyuBaseCharacter::JumpToAnimInstanceNode(FName Node)
{
    auto* AnimationInstance = GetOrCreateAnimInstance();
    AnimationInstance->JumpToNode(Node);
}

void ARyuBaseCharacter::SetCharacterMovementState(ERyuMovementState MovementState)
{
    CharacterMovementState = MovementState;
}

/* ONLY FOR DEBUGGING*/
void ARyuBaseCharacter::SwitchCharacterStateEnum(ERyuCharacterState InputState)
{
    if (CharacterState != nullptr)
    {
        ERyuCharacterState CharState = CharacterState->GetState();

        switch (CharState)
        {
            case ERyuCharacterState::Idle:
            {
                URyuCharacterOnGroundState* OnGroundState =
                    Cast<URyuCharacterOnGroundState>(CharacterState);
                if (OnGroundState)
                {
                    OnGroundState->SetCharacterState(ERyuCharacterState::JumpForward);
                }

                break;
            }
            case ERyuCharacterState::Run:
            {
                URyuCharacterOnGroundState* OnGroundState =
                    Cast<URyuCharacterOnGroundState>(CharacterState);
                if (OnGroundState)
                {
                    OnGroundState->SetCharacterState(ERyuCharacterState::JumpEnd);
                    //OnGroundState->SetCharacterState(ERyuCharacterState::JumpForwardFast);
                }
                break;
            }
            case ERyuCharacterState::JumpUpward:
            case ERyuCharacterState::JumpForward:
            case ERyuCharacterState::JumpForwardFast:
            case ERyuCharacterState::JumpBackward:
            case ERyuCharacterState::JumpBackwardFast:
            {
                URyuCharacterJumpState* JumpState = Cast<URyuCharacterJumpState>(CharacterState);
                if (JumpState)
                {
                    JumpState->SetCharacterState(ERyuCharacterState::JumpEnd);
                }
                break;
            }
            case ERyuCharacterState::JumpEnd:
            {
                URyuCharacterJumpState* JumpState = Cast<URyuCharacterJumpState>(CharacterState);
                if (JumpState)
                {
                    JumpState->SetCharacterState(ERyuCharacterState::Idle);
                }
                break;
            }

            default:
                break;
        }
    }
}

void ARyuBaseCharacter::SetAllowReleaseAxisKey(bool AllowState)
{
    bAllowReleaseAxisKey = AllowState;
}

void ARyuBaseCharacter::OnHandleCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                    AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp,
                                                    int32 OtherBodyIndex, bool bFromSweep,
                                                    const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleBeginOverlap(): CapslOvlp in from %s"),
           *OtherComp->GetName());
}

void ARyuBaseCharacter::OnHandleCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                                  AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp,
                                                  int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Log, TEXT("OnHandleCapsuleEndOverlap(): CapslOvlp out from %s"),
           *OtherComp->GetName());
}

bool ARyuBaseCharacter::CheckOverlapClimbableActors()
{
    // TODO ClimbStateMach genauer abchecken nach was geprueft wird: a) climbingActors b) pushactors c) blabla ...

    UE_LOG(LogTemp, Log, TEXT("CheckOverlappingActors(): "));

    auto* ClimbingComp = FindComponentByClass<URyuClimbingComponent>();

    if (!ClimbingComp)
    {
        return false;
    }

    GetOverlappingActors(CapsuleOverlappedActors, ClimbingComp->ClimbableActorClass);

    if (auto* DebugComp = FindComponentByClass<URyuDebugComponent>())
    {
        DebugComp->OutputCapsuleOverlappedComponents(CapsuleOverlappedComponents);
    }

    if (CapsuleOverlappedActors.Num() > 0)
    {
        return CheckOverlappingComponents();
    }
    else
    {
        ClimbingComp->ResetClimbingState();
        return false;
    }
}

bool ARyuBaseCharacter::FindCurrentInputState(ERyuInputState InputState)
{
    return CurrentInputs.Contains(InputState);
}

/*
Atm this only function when max. 2 climbing trigger are in place. because we automatically want to choose the correct one (up/down, left/right)
*/
// check if an overlPPING actor is there or not and in InputMethod set the state
bool ARyuBaseCharacter::CheckOverlappingComponents()
{
    //BY DESIGN: max. overlapped climbing Triggers == 2
    //check if overlapped comp right or left
    int CountClimbTrigger = CapsuleOverlappedActors.Num();

    if (CountClimbTrigger == 0)
    {
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): %s"),
           *FString::FromInt(CountClimbTrigger));

    if (CountClimbTrigger > 2)
    {
        UE_LOG(LogTemp, Warning,
               TEXT("CheckOverlappingComponents(): To many overlapping ClimbingTrigger (%s). "
                    "Please realign ClimbingAssets!"),
               *FString::FromInt(CountClimbTrigger));
        UE_LOG(LogTemp, Warning, TEXT("CheckOverlappingComponents(): Trigger are:"));
        for (int i = 0; i < CountClimbTrigger; i++)
        {
            UE_LOG(LogTemp, Log, TEXT("CheckOverlappingComponents(): %s"),
                   *CapsuleOverlappedActors[i]->GetName());
        }
        return false;
    }
    else
    {
        auto* ClimbingComp = FindComponentByClass<URyuClimbingComponent>();

        if (!ClimbingComp)
        {
            return false;
        }

        ERYULedgeSideEntered LedgeSide = ClimbingComp->GetLedgeSide(0);

        if (CountClimbTrigger == 1)
        {
            //save all Overlapping Components in a array, before that we make sure that the array is empty
            //we know that the TriggerComponents we search for, are UBoxComponents and we do not need any other
            ClimbingComp->GetOverlappingBoxComponents();

            UE_LOG(LogTemp, Log,
                   TEXT("CheckOverlappingComponents(): Yes it´s %s with the Trigger at ... "),
                   *CapsuleOverlappedActors[0]->GetName());
            /* fast check what kind of trigger*/

            if (auto* DebugComp = FindComponentByClass<URyuDebugComponent>())
            {
                DebugComp->OutputCapsuleOverlappedComponents(CapsuleOverlappedComponents);
            }

            if (LedgeSide == ERYULedgeSideEntered::LeftSide)
            {
                UE_LOG(LogTemp, Log, TEXT("... the left Side of the Actor.."));
            }

            if (LedgeSide == ERYULedgeSideEntered::RightSide)
            {
                UE_LOG(LogTemp, Log, TEXT("... the right Side of the Actor."));
            }
        }

        if (CountClimbTrigger == 2)
        {
            //Check at Input: we know we can Move Up OR Down
            //check if overlapped comp up or/and down
            //check if overlapped comp right or left

            // TODO later call to Character StateMachine !
            ClimbingComp->SetClimbingState(ERYUClimbingMode::CANCLIMBUPANDDOWN);
        }
        return true;
    }
}

void ARyuBaseCharacter::SetClimbingMode(ERYUClimbingMode ClimbingModeToSet)
{
    if (auto* ClimbComp = FindComponentByClass<URyuClimbingComponent>())
    {
        ClimbComp->SetClimbingState(ClimbingModeToSet);
    }
}

void ARyuBaseCharacter::SetLookRight()
{
    FVector currV = GetVelocity();

    if (currV.X > 0)
    {
        LookDirection = ERyuLookDirection::Right;
        //bLookRight = true;
    }
    else if (currV.X < 0)
    {
        LookDirection = ERyuLookDirection::Left;
        //bLookRight = false;
    }
}

ERYUClimbingMode ARyuBaseCharacter::GetClimbingMode()
{
    if (auto* ClimbComp = FindComponentByClass<URyuClimbingComponent>())
    {
        return ClimbComp->GetClimbingState();
    }

    return ERYUClimbingMode::NONE;
}

TArray<ERyuInputState> ARyuBaseCharacter::GetCurrentInputStates()
{
    return CurrentInputs;
}

float ARyuBaseCharacter::GetCharacterStatus(ERyuCharacterStatus Status)
{
    switch (Status)
    {
        case ERyuCharacterStatus::Stamina:
        {
            // TODO Placeholder -> later ask another StatusComponent of CharacterStatus...
            return 100.0f;
            break;
        }
        default:
            return 0.0f;
            break;
    }
}

ERyuInteractionStatus ARyuBaseCharacter::GetInteractionStatus()
{
    // TODO GetInteractionStatus resp. which Objects/Person is colliding; else return None
    // if Movable Object: InteractionStatus = StartMoveObjects
    return InteractionStatus;
}

ERyuCharacterState ARyuBaseCharacter::GetLastCharacterState()
{
    return LastCharacterState;
}

FJumpStartValues ARyuBaseCharacter::GetJumpStartValues()
{
    return JumpStartValues;
}

ERyuMovementState ARyuBaseCharacter::GetCharacterMovementState()
{
    return CharacterMovementState;
}

ERyuLookDirection ARyuBaseCharacter::GetLookDirection()
{
    //return bLookRight;
    return LookDirection;
}

ERyuMoveRightAxisInputState ARyuBaseCharacter::GetMoveRightAxisState()
{
    return MoveRightAxisState;
}

ERyuMoveUpAxisInputState ARyuBaseCharacter::GetMoveUpAxisState()
{
    return MoveUpAxisState;
}

ERyuCharacterPossibility ARyuBaseCharacter::GetCharacterPossibility()
{
    return CharacterPosibility;
}

URyuMovementComponent* ARyuBaseCharacter::GetRyuCharacterMovement()
{
    if (auto* MovementComp = Cast<URyuMovementComponent>(GetCharacterMovement()))
    {
        return MovementComp;
    }
    else
    {
        return nullptr;
    }
}

float ARyuBaseCharacter::GetFallVelocityZFromJump()
{
    return GetRyuCharacterMovement()->JumpSwitchToFallVelocityZ;
}

float ARyuBaseCharacter::GetFallNormalVelocityZ()
{
    return GetRyuCharacterMovement()->FallNormalVelocityZ;
}

float ARyuBaseCharacter::GetFallDeepVelocityZ()
{
    return GetRyuCharacterMovement()->FallDeepVelocityZ;
}

float ARyuBaseCharacter::GetFallToDeathVelocityZ()
{
    return GetRyuCharacterMovement()->FallToDeathVelocityZ;
}

void ARyuBaseCharacter::StartJumpingTimer()
{
    UE_LOG(LogTemp, Warning, TEXT("JumpTimer started"));
    GetWorldTimerManager().SetTimer(GetRyuCharacterMovement()->Timerhandle_BeginJump, this,
                                    &ARyuBaseCharacter::StartJump,
                                    GetRyuCharacterMovement()->TimerStartJump, false);
}

void ARyuBaseCharacter::StartJump()
{
    UE_LOG(LogTemp, Warning, TEXT("Jump executed"));
    bPressedJump = true;
}

void ARyuBaseCharacter::HandleInput(ERyuInputState Input)
{
    bHandleInput = true;

    switch (Input)
    {
        case ERyuInputState::PressLeft:
        {
            MoveRightAxisState = ERyuMoveRightAxisInputState::PressLeftAxisKey;
            break;
        }
        case ERyuInputState::PressRight:
        {
            MoveRightAxisState = ERyuMoveRightAxisInputState::PressRightAxisKey;
            break;
        }
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseRight:
        {
            MoveRightAxisState = ERyuMoveRightAxisInputState::Inactive;
        }
        case ERyuInputState::PressDown:
        {
            MoveUpAxisState = ERyuMoveUpAxisInputState::PressDownAxisKey;
            break;
        }
        case ERyuInputState::PressUp:
        {
            MoveUpAxisState = ERyuMoveUpAxisInputState::PressUpAxisKey;
            break;
        }
        case ERyuInputState::ReleaseUp:
        case ERyuInputState::ReleaseDown:
        {
            MoveUpAxisState = ERyuMoveUpAxisInputState::Inactive;
            break;
        }
        default:
            break;
    }

    if ((CharacterState == nullptr) || (Input == ERyuInputState::None))
    {
        return;
    }

    // save the pressed Input of the current State for other Methods besides HandleIput
    CharacterState->SetInputPressedState(Input);

    // whether to add or remove ButtonCommand from CurrentInputs-Array
    CheckCurrentInputState(Input);

    // Mainly due AnimationEndedInput this needs to be implemented here in the BaseClass
    //URyuCharacterState* state = nullptr;

    UE_LOG(LogRyu, Error, TEXT("RYUBASE: HANDLEINPUT: %s"),
           *URyuStaticFunctionLibrary::InputStateToString(Input));
    // TODO Possible fix for crash ?  check if everyhandleInput returns a NEW State OR the current one! not nullptr !!! --> memory leak
    URyuCharacterState* state = CharacterState->HandleInput(this, Input);

    //UE_LOG(LogRyu, Log, TEXT("CharBase | ResetTimer: CharacterSpeed: %s "),
    //       *this->GetCharacterMovement()->Velocity.ToString());

    if ((state == nullptr) || (state == CharacterState))
    {
        return;
    }

    // save the pressed Input of the current State to the new state for other Methods besides HandleIput
    state->SetInputPressedState(Input);
    /*
	UE_LOG(LogRyu, Warning, TEXT("GetCurrentCharState: %s"),
           *URyuStaticFunctionLibrary::CharacterStateToString(CharacterState->GetState()));
    UE_LOG(LogRyu, Warning, TEXT("GetInputState: %s"),
           *URyuStaticFunctionLibrary::InputStateToString(CharacterState->GetInputPressedState()));
	*/

    // Call Exit-Action on the old state
    CharacterState->Exit(this);
    // TODO: check if this really dealloc every state or  only the states which are changed by playerinput
    // UE_LOG(LogRyu, Log, TEXT("Destroying Old State..."));
    CharacterState->ConditionalBeginDestroy();
    CharacterState = nullptr;
    //EquipmentState->Exit(this);
    // we really need to delete NewObjects<OLDSTATE> or mark for GC, otherwise MemoryLeak ?
    // delete old CharacterState;
    CharacterState = state;
    // EquipmentState = state;
    // UE_LOG(LogRyu, Log, TEXT("OldInputpressedState: %s"),
    //       *URyuStaticFunctionLibrary::InputStateToString(state->GetInputPressedState()));
    CharacterState->SetInputPressedState(state->GetInputPressedState());
    // Call the enter Action on the new State
    CharacterState->Enter(this);
    //EquipmentState->Enter(this);
    bHandleInput = false;
}

void ARyuBaseCharacter::InitializeCommands()
{
    // Pure Mapping from Input to Buttons
    Commands.Add(TEXT("Ability"), NewObject<URyuAbilityCommand>());
    Commands.Add(TEXT("Block"), NewObject<URyuBlockCommand>());
    Commands.Add(TEXT("Duck"), NewObject<URyuDuckCommand>());
    Commands.Add(TEXT("HighStrike"), NewObject<URyuHighStrikeCommand>());
    Commands.Add(TEXT("Interact"), NewObject<URyuInteractCommand>());
    Commands.Add(TEXT("ItemSelect"), NewObject<URyuItemSelectCommand>());
    Commands.Add(TEXT("Jump"), NewObject<URyuJumpCommand>());
    Commands.Add(TEXT("LowStrike"), NewObject<URyuLowStrikeCommand>());
    Commands.Add(TEXT("MoveRight"), NewObject<URyuMoveRightCommand>());
    Commands.Add(TEXT("ShardSelect"), NewObject<URyuShardSelectCommand>());
    Commands.Add(TEXT("Sneak"), NewObject<URyuSneakCommand>());
    Commands.Add(TEXT("Sprint"), NewObject<URyuSprintCommand>());
}

void ARyuBaseCharacter::InitInputCounterparts()
{
    if (KeyInputCounterpartTable)
    {
        // Make a Map (TMap:key=<ButtonPress>) from the DataTable to simply get Access to the given Release-Counterpart of the ButtonKeyPressAction
        for (auto it : KeyInputCounterpartTable->GetRowMap())
        {
            // simply cast the Value to the StructType of the DataTable
            FInputCounterparts* data = (FInputCounterparts*)(it.Value);
            KeyInputCounterpartMap.Add(data->ReleaseKeyState, *data);
        }
    }
}

bool ARyuBaseCharacter::CheckCharacterEnumValue()
{
    // TODO: why da hell is this here ? this prevent Update-Calls in CharacterStates which are note listed in the true section !
    if ((CharacterState->GetState() == ERyuCharacterState::Idle)
        || (CharacterState->GetState() == ERyuCharacterState::JumpForward)
        || (CharacterState->GetState() == ERyuCharacterState::JumpForwardFast)
        || (CharacterState->GetState() == ERyuCharacterState::JumpUpward)
        || (CharacterState->GetState() == ERyuCharacterState::JumpEnd)
        || (CharacterState->GetState() == ERyuCharacterState::Run)
        || (CharacterState->GetState() == ERyuCharacterState::FallingNormal)
        || (CharacterState->GetState() == ERyuCharacterState::FallingDeep)
        || (CharacterState->GetState() == ERyuCharacterState::FallingEnd)
        || (CharacterState->GetState() == ERyuCharacterState::FallingNormalEnd)
        || (CharacterState->GetState() == ERyuCharacterState::FallingDeepEnd)
        || (CharacterState->GetState() == ERyuCharacterState::FallingDeath))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ARyuBaseCharacter::SetEndJumpTimer()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(EndJumpTimerHandle) == false)
    {
        auto* MoveComp = Cast<URyuMovementComponent>(GetCharacterMovement());
        if (MoveComp)
        {
            GetWorld()->GetTimerManager().SetTimer(EndJumpTimerHandle, this,
                                                   &ARyuBaseCharacter::ResetEndJumpTimer,
                                                   MoveComp->TimerEndJump, false);
        }
    }
}

void ARyuBaseCharacter::SetFallingTimer()
{
    UE_LOG(LogTemp, Log, TEXT("SetFallingTimer."));
    auto* MoveComp = Cast<URyuMovementComponent>(GetCharacterMovement());
    if (MoveComp)
    {
        /* GetWorld()->GetTimerManager().SetTimer(FallingTimerHandle, this,
                                               &ARyuBaseCharacter::IncreaseFallingVelocity,
                                               MoveComp->FallingVelocityTimer, true);
											   */
    }
}

void ARyuBaseCharacter::SetVelocityAfterJump()
{
    // TODO: set vel and acc after jump
    if (auto* MoveComp = Cast<URyuMovementComponent>(GetCharacterMovement()))
    {
        //MoveComp->SetVelocityAfterJump(JumpStartValues);
    }
}

bool ARyuBaseCharacter::IsInCombat()
{
    //TODO check if Character is in a Combat-Situation
    return false;
}

bool ARyuBaseCharacter::IsFalling()
{
    return GetRyuCharacterMovement()->IsFalling();
}

bool ARyuBaseCharacter::EnemyInSight()
{
    return false;
}

void ARyuBaseCharacter::FlipCharacter()
{
    if (Controller != nullptr)
    {
        //if (TravelDirection < 0.0f)
        if (LookDirection == ERyuLookDirection::Right)
        {
            Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
            //CameraBoom->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
            LookDirection = ERyuLookDirection::Left;
        }
        //else if (TravelDirection > 0.0f)
        else
        {
            Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
            //CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
            LookDirection = ERyuLookDirection::Right;
        }
    }

    // LookDirection = !LookDirection;
    UE_LOG(LogTemp, Log, TEXT("FlipCharacter(): lookRight = %s"),
           bLookRight ? TEXT("true") : TEXT("false"));
    //coa vs reset ?
    CheckOverlapClimbableActors();
}

ERyuCharacterState ARyuBaseCharacter::GetCharacterStateEnum()
{
    if (CharacterState == nullptr)
    {
        ERyuCharacterState state = ERyuCharacterState::None;
        return state;
    }
    return CharacterState->GetState();
}

URyuCharacterState* ARyuBaseCharacter::GetCharacterState()
{
    return CharacterState;
}

ERyuInputState ARyuBaseCharacter::GetInputState()
{
    if (CharacterState == nullptr)
    {
        ERyuInputState state = ERyuInputState::None;
        return state;
    }
    return CharacterState->GetInputPressedState();
}

void ARyuBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(
        this, &ARyuBaseCharacter::OnHandleCapsuleBeginOverlap);
    GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(
        this, &ARyuBaseCharacter::OnHandleCapsuleEndOverlap);
    InitInputCounterparts();

    IdleCapsuleHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    IdleSpriteRelativePosition = GetSprite()->GetRelativeLocation();
}

void ARyuBaseCharacter::ConfigurePlayer_Implementation(UPaperZDAnimPlayer* Player)
{
    Super::ConfigurePlayer_Implementation(Player);

    UPaperZDAnimInstance* AnimInstance2D = GetOrCreateAnimInstance();

    Player->OnPlaybackSequenceComplete.AddDynamic(this, &ARyuBaseCharacter::AnimationSequenceEnded);
}

void ARyuBaseCharacter::Jump()
{
}

void ARyuBaseCharacter::SetJumpAllowed(bool JumpState)
{
    GetRyuCharacterMovement()->SetJumpAllowedState(JumpState);
}

void ARyuBaseCharacter::StopJumping()
{
}

void ARyuBaseCharacter::SetLookDirection(ERyuLookDirection Direction)
{
    LookDirection = LookDirection;
}

void ARyuBaseCharacter::SetLastCharacterState(ERyuCharacterState LastState)
{
    LastCharacterState = LastState;
}
