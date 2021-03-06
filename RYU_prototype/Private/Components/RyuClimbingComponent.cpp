// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuClimbingComponent.h"
#include "Character/RYU2DENUM_ClimbingMode.h"
#include "Character/RYU2DENUM_Movement.h"
#include "Character/RyuMainCharacter.h"
#include "Components/RyuDebugComponent.h"
#include "Components/RyuMovementComponent.h"
#include "Components/RyuTimelineComponent.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RyuBaseCharacter.h"
#include <Components/BoxComponent.h>

// Sets default values for this component's properties
URyuClimbingComponent::URyuClimbingComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    InitializeValues();
    // ...
}

void URyuClimbingComponent::ChangeClimbingMode()
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

void URyuClimbingComponent::CheckMoveUpState(float MoveUpInput)
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

void URyuClimbingComponent::Climb(float MoveUpInput)
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
                // TODO Without a Timeline but we need to adjust the Pivotpoint in EVERY Frame in the FrameAssets, this SUCKS! maybe i can automatisize it when making my own animations in PS/Aesprite/PixelArtProg
                // (like we did in PS & Visionaire with a positioning exportfile?)

                // TODO solve this by Broadcasting an Event for ComponentCommunication / CSTM relevant
                if (auto* MovementComp = GetOwner()->FindComponentByClass<URyuMovementComponent>())
                {
                    MovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(
                                                                   ERYUClimbingMode::CLIMBUPLEDGE));
                }

                RYUClimbingMode = ERYUClimbingMode::CLIMBUPLEDGE;
            }
            break;
        default:
            break;
    }
}

ERYUClimbingMode URyuClimbingComponent::GetClimbingState()
{
    return RYUClimbingMode;
}

UBoxComponent* URyuClimbingComponent::GetOverlappedClimbingComponent(ERYULedgePosition2D LedgePosi)
{
    auto* CharOwner = Cast<ARyuMainCharacter>(GetOwner());
    if (CharOwner == nullptr)
    {
        return nullptr;
    }

    //make sure only relevant BoxTrigger are in the Componentsarray
    GetOverlappingBoxComponents();

    //sort Array resp. Componentlocation
    CharOwner->CapsuleOverlappedComponents.Sort([LedgePosi](auto& lhs, auto& rhs) {
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

    if (auto* DebugComp = CharOwner->FindComponentByClass<URyuDebugComponent>())
    {
        DebugComp->OutputCapsuleOverlappedComponents(CharOwner->CapsuleOverlappedComponents);
    }

    return CharOwner->CapsuleOverlappedComponents[0]
               ? Cast<UBoxComponent>(CharOwner->CapsuleOverlappedComponents[0])
               : nullptr;
}

UBoxComponent* URyuClimbingComponent::GetOverlappedClimbingComponent(FName UpOrDown,
                                                                     FName LeftOrRight)
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

    auto* CharOwner = Cast<ARyuMainCharacter>(GetOwner());
    if (CharOwner == nullptr)
    {
        return nullptr;
    }

    // uses Lambda-functionality / use of Algorithm (FindByPredicate); LERN DIE ALGORITHMEN ALDER SONST HANDKANTE ! (orig. MaxZitat XD )
    //Max:
    UPrimitiveComponent** Foo = nullptr;
    if (ClimbUpDownCheck == UpOrDown)
    {
        Foo = CharOwner->CapsuleOverlappedComponents.FindByPredicate(
            [&UpOrDown, &LeftOrRight](auto* x) {
                auto* OvCC = Cast<UBoxComponent>(x);
                return OvCC && (OvCC->ComponentTags[0] == UpOrDown)
                       && (OvCC->ComponentTags[1] == LeftOrRight);
            });
    }

    return Foo ? Cast<UBoxComponent>(*Foo) : nullptr;
}

void URyuClimbingComponent::SetClimbingState(ERYUClimbingMode ClimbState)
{
    RYUClimbingMode = ClimbState;
}

void URyuClimbingComponent::SetClimbingPostitionsAndMovementMode(EPlayerMovement PlayerMove,
                                                                 UBoxComponent* ClimbingTrigger)
{
    // TODO UtilityFunction / EventBroadcast Communication
	// TODO CSM
	/*
    auto* MainChar = Cast<ARyuMainCharacter>(GetOwner());
    auto* MovementComp = GetOwner()->FindComponentByClass<URyuMovementComponent>();
    auto* TimelineComp = GetOwner()->FindComponentByClass<URyuTimelineComponent>();

    if (MovementComp == nullptr)
    {
        return;
    }

	
    switch (PlayerMove)
    {
        case EPlayerMovement::JUMPUP: {
            //Use a Timeline for Positioning the Up-Jumps
            //MainChar->PlayerMovement = EPlayerMovement::JUMPUP;
            // TODO Broadcast Message
            if (TimelineComp)
            {
                //** Initialize the Start End Endpoints
                TimelineComp->StartTimelineLocation = MainChar->GetActorLocation();
                TimelineComp->EndTimelineLocation =
                    FVector(TimelineComp->StartTimelineLocation.X,
                            TimelineComp->StartTimelineLocation.Y,
                            TimelineComp->StartTimelineLocation.Z + TimelineComp->ClimbUpOffsetZ);
                MovementComp->SetMovementMode(MOVE_Custom,
                                              static_cast<uint8>(ERYUClimbingMode::JUMPTOLEDGE));
                TimelineComp->SetCurrentTimelineParamsFloat(nullptr, TimelineComp->JumpUpFloatCurve,
                                                            false, true);
                TimelineComp->PlayTimeline();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No RyuTimelineComponent found on Character %"),
                       *MainChar->GetName());
            }

            break;
        }

        case EPlayerMovement::JUMPSTART: {
            FVector CurrPlayerLocation = MainChar->GetActorLocation();
            float CurJumpForwardDistance = 0;
            // TODO: find Direction of arrowcomponent (see climbupledgestates)
            //ArrowComponent->GetComponentLocation(). ? (CurJumpForwardDistance = MovementComp->JumpForwardDistance) : (CurJumpForwardDistance = MovementComp->JumpForwardDistance) *(-1);

            TimelineComp->StartTimelineLocation = CurrPlayerLocation;
            TimelineComp->EndTimelineLocation =
                FVector(CurrPlayerLocation.X, CurrPlayerLocation.Y, CurrPlayerLocation.Z);
        }

        case EPlayerMovement::CLIMBING: {
            UE_LOG(LogTemp, Log, TEXT("MoveUpState(Up): ClimbTriggerBox is %s : from Owner: %s "),
                   *ClimbingTrigger->GetName(), *ClimbingTrigger->GetOwner()->GetName());
            //TODO: search correct Actor, check if char needs to be flipped
            if ((MainChar->PlayerMovement != EPlayerMovement::CLIMBING)
                && MainChar->CheckFlipOverlappedActor(ClimbingTrigger))
            {
                MainChar->FlipCharacter();
            }
            MainChar->PlayerMovement = EPlayerMovement::CLIMBING;
            //sets the Movement of the player and approp. Specs
            UE_LOG(LogTemp, Log, TEXT("SetClimbingPostitionsAndMovementMode(): only once !"));
            SetClimbingPositions(ClimbingTrigger);
            //if (RYUClimbingMode == ERYUClimbingMode::CLIMBDOWNLEDGE)
            if (MainChar->GetMoveUpInput() < 0)
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
	*/
}

//TODO: can be summarized, take a closer look ....
void URyuClimbingComponent::SetClimbingPositions(UBoxComponent* ClimbTrigger)
{
    ARYUClimbingActor* ARY = Cast<ARYUClimbingActor>(ClimbTrigger->GetOwner());
    auto* MainChar = Cast<ARyuMainCharacter>(GetOwner());

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
                || ((RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN)
                    && (MainChar->GetMoveUpInput() < 0)))
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
                || ((RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN)
                    && (MainChar->GetMoveUpInput() > 0)))
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
                || ((RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN)
                    && (MainChar->GetMoveUpInput() < 0)))
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
                || ((RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN)
                    && (MainChar->GetMoveUpInput() > 0)))
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
        MainChar->SetActorLocation(PosChar);
    }
}

// Called when the game starts
void URyuClimbingComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

FVector URyuClimbingComponent::GetLedgeHangPosition()
{
    return LedgeHangPosition;
}

/*
param posi - Position in the OverlappedComponentArray to receive the correct TriggerBox
*/
ERyuLedgePosition URyuClimbingComponent::GetLedgePosition()
{
    auto* MainChar = Cast<ARyuMainCharacter>(GetOwner());
    ERyuLedgePosition LedgePosi;

    // if character can Climb Up AND Down the Ledges are at Top and Bottom from the char
    if (RYUClimbingMode == ERYUClimbingMode::CANCLIMBUPANDDOWN)
    {
        LedgePosi = ERyuLedgePosition::PosiUpDown;
        CurrentClimbTagName = CanClimbUpDownName;
        UE_LOG(LogTemp, Log, TEXT("GetLedgePosition(): PositionUpAndDown"));
    }
    else
    {
        //By design we Tag the Side in Array: ComponentTags[0]
        // if there is only ony ledge to climb we need to find the correct trigger ! (CapsuleOverlappedComponents[posi])
        if (MainChar->CapsuleOverlappedComponents[0]->ComponentTags[0] == CanClimbUpTagName)
        {
            LedgePosi = ERyuLedgePosition::PosiUp;
            CurrentClimbTagName = CanClimbUpTagName;
            UE_LOG(LogTemp, Log, TEXT("GetLedgePosition(): PositionUp"));
        }
        else if (MainChar->CapsuleOverlappedComponents[0]->ComponentTags[0] == CanClimbDownTagName)
        {
            LedgePosi = ERyuLedgePosition::PosiDown;
            CurrentClimbTagName = CanClimbDownTagName;
            UE_LOG(LogTemp, Log, TEXT("GetLedgePosition(): PositionDown"));
        }
        else
        {
            UE_LOG(LogTemp, Log,
                   TEXT("GetLedgeSide(): Please check if ComponentTagName[0] of Trigger %s from %s "
                        "is: 'CanClimbUp' or 'CanClimbDown'."),
                   *MainChar->CapsuleOverlappedComponents[0]->GetName(),
                   *MainChar->CapsuleOverlappedComponents[0]->GetOwner()->GetName());
            LedgePosi = ERyuLedgePosition::None;
        }
    }
    return LedgePosi;
}

ERYULedgeSideEntered URyuClimbingComponent::GetLedgeSideEntered()
{
    return ESideEntered;
}

ERYULedgeSideEntered URyuClimbingComponent::GetLedgeSide(int posi)
{
    auto* MainChar = URyuStaticFunctionLibrary::GetMainCharOwner(this);

    if (MainChar == nullptr)
    {
        return ERYULedgeSideEntered::NONE;
    }

    //needs to be checked when getting input, the we need to decide if we flip the char or not
    ERYULedgeSideEntered LedgeSideEntered;
    //By design we Tag the Side in Array: ComponentTags[1]
    if (MainChar->CapsuleOverlappedComponents[posi]->ComponentTags[1].IsEqual(LeftLedgePosiTagName))
    {
        LedgeSideEntered = ERYULedgeSideEntered::LeftSide;
        CurrentLedgePosiTagName = LeftLedgePosiTagName;
    }
    else if (MainChar->CapsuleOverlappedComponents[posi]->ComponentTags[1].IsEqual(
                 RightLedgePosiTagName))
    {
        LedgeSideEntered = ERYULedgeSideEntered::RightSide;
        CurrentLedgePosiTagName = RightLedgePosiTagName;
    }
    else
    {
        UE_LOG(LogTemp, Log,
               TEXT("GetLedgeSide(): Please check if ComponentTagName[1] of Trigger %s from %s is: "
                    "'Right' or 'Left'."),
               *MainChar->CapsuleOverlappedComponents[posi]->GetName(),
               *MainChar->CapsuleOverlappedComponents[posi]->GetOwner()->GetName());

        LedgeSideEntered = ERYULedgeSideEntered::NONE;
    }
    return LedgeSideEntered;
}

void URyuClimbingComponent::GetOverlappingBoxComponents()
{
    if (auto* MainChar = URyuStaticFunctionLibrary::GetMainCharOwner(this))
    {
        MainChar->GetOverlappingComponents(MainChar->CapsuleOverlappedComponents);

        MainChar->CapsuleOverlappedComponents.RemoveAll(
            [](auto* elem) { return !elem->IsA<UBoxComponent>(); });
    }
}

void URyuClimbingComponent::ToggleEnterLedgeSide()
{
    UE_LOG(LogTemp, Log, TEXT("ToggleEnterLedgeSide(): %s"), *CurrentLedgePosiTagName.ToString());
    if (CurrentLedgePosiTagName == LeftLedgePosiTagName)
    {
        CurrentLedgePosiTagName = RightLedgePosiTagName;
    }
    else
    {
        CurrentLedgePosiTagName = LeftLedgePosiTagName;
    }
}

void URyuClimbingComponent::SetLedgeHangPosition(FVector LedgeTargetPoint, FName LedgeSide)
{
    LedgeHangPosition = LedgeTargetPoint;
    //relict from 3D CharHangPosition !
    LedgeHangPosition.Z = LedgeHangPosition.Z + 115.0f;

    //	LeftPositionTag = "Left";
    //RightPositionTag = "Right";

    if (LedgeSide == "Left")
    {
        UE_LOG(LogTemp, Log, TEXT("SetLedgeHangPosition(): Left Side Ledge."));
        ESideEntered = ERYULedgeSideEntered::LeftSide;
    }
    else
    {
        if (LedgeSide == "Right")
        {
            UE_LOG(LogTemp, Log, TEXT("SetLedgeHangPosition(): Right Side Ledge."));
            ESideEntered = ERYULedgeSideEntered::RightSide;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("SetLedgeHangPosition(): no Side Ledge."));
            ESideEntered = ERYULedgeSideEntered::NONE;
        }
    }
}

void URyuClimbingComponent::ResetClimbingState()
{
    auto* MainChar = Cast<ARyuMainCharacter>(GetOwner());

	//TODO CSM
    //MainChar->PlayerMovement = EPlayerMovement::STAND;
    //RYUClimbingMode = ERYUClimbingMode::NONE;
    CurrentLedgePosiTagName = "";
    CurrentClimbTagName = "";
}

void URyuClimbingComponent::InitializeValues()
{
    ESideEntered = ERYULedgeSideEntered::NONE;

    //Constant
    CanClimbUpTagName = "CanClimbUp";
    CanClimbDownTagName = "CanClimbDown";
    CanClimbUpDownName = "CanClimbUpAndDown";

    LeftLedgePosiTagName = "Left";
    RightLedgePosiTagName = "Right";
}

// Called every frame
void URyuClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}
