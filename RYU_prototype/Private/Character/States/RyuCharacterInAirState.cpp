// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterInAirState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterJumpState.h"

URyuCharacterInAirState::URyuCharacterInAirState()
{
}

URyuCharacterState* URyuCharacterInAirState::HandleInput(ARyuBaseCharacter* Character,
                                                         const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::AnimationEnded:
        {
            return InputAnimationEnded(Character); //InputAnimationEnde(Character);
			break;
        }
        default:
        {
            return Character->GetCharacterState();
            break;
        }
    }
}

void URyuCharacterInAirState::Update(ARyuBaseCharacter* Character)
{
}

URyuCharacterState* URyuCharacterInAirState::InputAnimationEnded(ARyuBaseCharacter* Character)
{
    return this;
}
