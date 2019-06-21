// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterSprintState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterSprintState::URyuCharacterSprintState()
{
}

IRyuCharacterState* URyuCharacterSprintState::HandleInput(ARyuBaseCharacter* Character,
                                                          const ERyuInputState Input)
{
    if (Input == ERyuInputState::PressJump)
    {
		// if Back is pressed fast
		//return NewObject<URyuCharacterJumpBackwardState>();
        return NewObject<URyuCharacterJumpForwardState>();
    }
    else
    {
        // only make special call when Input occurs which is not handled in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
        return Super::HandleInput(Character, Input);
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
