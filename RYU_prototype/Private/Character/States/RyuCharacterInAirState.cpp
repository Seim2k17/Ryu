// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterInAirState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterJumpState.h"

URyuCharacterInAirState::URyuCharacterInAirState()
{
}

IRyuCharacterState* URyuCharacterInAirState::HandleInput(ARyuBaseCharacter* Character,
                                                         const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::AnimationEnded:
        {
            return InputAnimationEnded(Character); //InputAnimationEnde(Character);
        }
        default:
        {
            return nullptr;
            break;
        }
    }
}

void URyuCharacterInAirState::Update(ARyuBaseCharacter* Character)
{
}

IRyuCharacterState* URyuCharacterInAirState::InputAnimationEnded(ARyuBaseCharacter* Character)
{
    return nullptr;
}
