// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterEndClimbState.h"
#include "Character/RyuMainCharacter.h"
#include "Enums/ERyuCharacterState.h"
#include "States/RyuCharacterIdleState.h"

URyuCharacterEndClimbState::URyuCharacterEndClimbState()
{
}

URyuCharacterState* URyuCharacterEndClimbState::HandleInput(ARyuBaseCharacter* Character,
                                                            const ERyuInputState Input)
{
    switch (Input)
    {
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

void URyuCharacterEndClimbState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterEndClimbState::Enter(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("ClimbingEndState: ENTER: "));
    CharacterState = ERyuCharacterState::ExitLadder;

    //Character->JumpToAnimInstanceNode(Character->IdleNodeName);
    if (auto MainChar = Cast<ARyuMainCharacter>(Character))
    {
        MainChar->GetRyuCharacterMovement()->ResetClimbingState();
    }

    /* ToDo: Check which Climbing-Type end shere !
    switch (CharacterState)
    {
        case ERyuCharacterState::
        case ERyuCharacterState::ClimbDownLadder:
        {
            UE_LOG(LogRyu, Log, TEXT("ClimbingEndState: EXIT LADDER: "));
            CharacterState = ERyuCharacterState::ExitLadder;
            break;
        }
        default:
            break;
    }
    */
}

void URyuCharacterEndClimbState::Exit(ARyuBaseCharacter* Character)
{
}
