// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterClimbLadderState.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "GameWorld/Interactibles/RyuLadderBase.h"
#include "States/RyuCharacterEndClimbState.h"
#include "States/RyuCharacterIdleState.h"
#include "States/RyuCharacterRunState.h"
#include "RyuMainCharacter.h"

URyuCharacterClimbLadderState::URyuCharacterClimbLadderState()
{
}

URyuCharacterState* URyuCharacterClimbLadderState::HandleInput(ARyuBaseCharacter* Character,
                                                               const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressUp:
        {
            if (false /* TODO after spherecast implementattion , reach end of the ladder*/)
            {
                UE_LOG(LogRyu, Log, TEXT("Character Exits the Ladder at the top."));
                CharacterState = ERyuCharacterState::ExitLadder;
                return this;
            }
            else
            {
                UE_LOG(LogRyu, Log, TEXT("Character ClimbsUp Ladder."));
                CharacterState = ERyuCharacterState::ClimbUpLadder;
                ClimbOutState = ELadderClimboutState::Top;
                return this;
            }

            break;
        }
        case ERyuInputState::PressDown:
        {
            UE_LOG(LogRyu, Log, TEXT("Character ClimbsDown Ladder."));
            CharacterState = ERyuCharacterState::ClimbDownLadder;
            ClimbOutState = ELadderClimboutState::Bottom;
            return this;
            break;
        }
        case ERyuInputState::ReleaseDown:
        case ERyuInputState::ReleaseUp:
        {
            UE_LOG(LogRyu, Log, TEXT("Character Idles on Ladder."));
            CharacterState = ERyuCharacterState::Idle;
            return this;
            break;
        }
        /* HANDLEINPUT NEEDS TO RETURN A NEW STATE ! */
        case ERyuInputState::InputEndClimbing:
        {
            return NewObject<URyuCharacterEndClimbState>();
        }

        case ERyuInputState::AnimationEnded:
        {
            return NewObject<URyuCharacterIdleState>();
        }

        default:
        {
            return Super::HandleInput(Character, Input);
            break;
        }
    }
}

void URyuCharacterClimbLadderState::Update(ARyuBaseCharacter* Character)
{
    Super::Update(Character);
    if (auto MainChar = Cast<ARyuMainCharacter>(Character))
    {
        float MoveUpInput = MainChar->GetMoveUpInput();
        MainChar->AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MoveUpInput);
    }
}

void URyuCharacterClimbLadderState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::EnterLadder;

    if (auto MainChar = Cast<ARyuMainCharacter>(Character))
    {
        auto Ladder = Cast<ARyuLadderBase>(MainChar->GetOverlappedActor());

        /*
        auto WorldLadderTransform = Ladder->ActorToWorld();
        FVector LadderPos = WorldLadderTransform.GetLocation();
        // TODO: different axis alignments of the actors^^ solve it ?
        // X-axis of climb-position == y-axis of actor, y-axis === x-axis / rotation around -90° z-axis of the ladder
        FVector StartClimb = Ladder->ClimbingDownLocation;
        FVector ClimbPos = LadderPos - (LadderPos.ForwardVector * StartClimb.X)
                           + (LadderPos.RightVector * StartClimb.Y);
                           */
        if (Ladder != nullptr)
        {
            FVector ClimbPos;
            // TODO the distances (where to teleport when climb out) needs to be adjustable acc. to the climbout - animation
            // --> into the ladderBaseActor !!! -> according to the direction where to climbout !
            if (Ladder->ClimbBottomOutLeft)
            {
                ClimbOutBtm = Ladder->ClimbingBottomLocation
                              - Ladder->ClimbOutBottomLeftOffset; //FVector(50, 0, -50);
            }
            else
            {
                ClimbOutBtm = Ladder->ClimbingBottomLocation
                              + Ladder->ClimbOutBottomRightOffset; //FVector(150, 0, 50);
            }

            if (Ladder->ClimbTopOutLeft)
            {
                ClimbOutTop = Ladder->ClimbingTopLocation
                              - Ladder->ClimbOutTopLeftOffset; //FVector(150, 0, -100);
            }
            else
            {
                ClimbOutTop = Ladder->ClimbingTopLocation
                              + Ladder->ClimbOutTopRightOffset; //FVector(100, 0, 100);
            }

            ClimbOutBtm.Y = MainChar->CharacterYPosition;
            ClimbOutTop.Y = MainChar->CharacterYPosition;

            if (MainChar->GetCharacterPossibility() == ERyuCharacterPossibility::CanClimbLadderUp)
            {
                ClimbPos = Ladder->ClimbingBottomLocation;
            }

            if (MainChar->GetCharacterPossibility() == ERyuCharacterPossibility::CanClimbLadderDown)
            {
                ClimbPos = Ladder->ClimbingTopLocation;
            }

            if (MainChar->GetCharacterPossibility()
                == ERyuCharacterPossibility::CanClimbLadderUpDown)
            {
                ClimbPos = Character->GetActorLocation();
            }

            ClimbPos.X = (Ladder->ClimbingBottomLocation.X + Ladder->ClimbingTopLocation.X
                          + Ladder->LadderBorderThickness)
                         / 2;
            ClimbPos.Y = MainChar->CharacterYPosition;
            MainChar->SetActorLocation(ClimbPos); // +));
        }

        Character->JumpToAnimInstanceNode(Character->ClimbinghNodeName);
        //set customMovementMode to flying or custom mode
        MainChar->GetRyuCharacterMovement()->SetMovementMode(
            MOVE_Custom, static_cast<uint8>(ERyuCharacterState::ClimbUpLadder));
        //SetMovementMode(MOVE_Custom, static_cast<uint8>(ERYUClimbingMode::HANGONLEDGE));
    }
}

void URyuCharacterClimbLadderState::Exit(ARyuBaseCharacter* Character)
{
    // TODO: set Character to LadderObject top or DownPosition
    if (auto MainChar = Cast<ARyuMainCharacter>(Character))
    {
        switch (ClimbOutState)
        {
            case ELadderClimboutState::Top:
            {
                MainChar->SetActorLocation(ClimbOutTop);
                UE_LOG(LogRyu, Log, TEXT("Character SetOnTop at: %s"), *ClimbOutTop.ToString());
                break;
            }

            case ELadderClimboutState::Bottom:
            {
                MainChar->SetActorLocation(ClimbOutBtm);
                UE_LOG(LogRyu, Log, TEXT("Character SetOnBtm at: %s"), *ClimbOutBtm.ToString());
                break;
            }
            default:
                break;
        }
    }
}
