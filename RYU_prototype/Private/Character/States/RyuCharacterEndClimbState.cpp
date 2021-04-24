// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterEndClimbState.h"
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
    switch (CharacterState)
    {
        case ERyuCharacterState::ClimbUpLadder:
        case ERyuCharacterState::ClimbDownLadder:
        {
            CharacterState = ERyuCharacterState::ExitLadder;
            break;
        }
        default:
            break;
    }
}

void URyuCharacterEndClimbState::Exit(ARyuBaseCharacter* Character)
{
}
