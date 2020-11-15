// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterClimbLadderState.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "States/RyuCharacterIdleState.h"
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
                return this;
            }

            break;
        }
        case ERyuInputState::PressDown:
        {
            UE_LOG(LogRyu, Log, TEXT("Character ClimbsDown Ladder."));
            CharacterState = ERyuCharacterState::ClimbDownLadder;
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
}
