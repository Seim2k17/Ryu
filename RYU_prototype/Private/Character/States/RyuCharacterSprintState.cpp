// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterSprintState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterJumpForwardState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterRollState.h"
#include "RyuCharacterRunState.h"

URyuCharacterSprintState::URyuCharacterSprintState()
{
}

IRyuCharacterState* URyuCharacterSprintState::HandleInput(ARyuBaseCharacter* Character,
                                                          const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressJump:
        {
            return NewObject<URyuCharacterJumpForwardState(ERyuMovementState::Sprinting)>();
            break;
        }
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseRight:
        {
            return NewObject<URyuCharacterIdleState>();
            break;
        }
        case ERyuInputState::ReleaseSprint:
        {
            return NewObject<URyuCharacterRunState>();
            break;
        }
        case ERyuInputState::PressDown:
        {
            return NewObject<URyuCharacterRollState>();
            break;
        }
        default:

            // only make special call when Input occurs which is not handled in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
            return Super::HandleInput(Character, Input);
            break;
    }

    return nullptr;
}

void URyuCharacterSprintState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterSprintState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::Sprint;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterSprintState::Exit(ARyuBaseCharacter* Character)
{
}
