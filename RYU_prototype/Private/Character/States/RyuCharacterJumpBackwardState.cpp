// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpBackwardState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMovementState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"

URyuCharacterJumpBackwardState::URyuCharacterJumpBackwardState()
{
}

URyuCharacterJumpBackwardState::URyuCharacterJumpBackwardState(ERyuMovementState MovementState)
    : CharacterMovementState(MovementState)
{
    //when char is sprinting and JumpBack is pressed she will make st like a double backjump to balance the fast movement
}

IRyuCharacterState* URyuCharacterJumpBackwardState::HandleInput(ARyuBaseCharacter* Character,
                                                                const ERyuInputState Input)
{
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
    return Super::HandleInput(Character, Input);
}

void URyuCharacterJumpBackwardState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterJumpBackwardState::Enter(ARyuBaseCharacter* Character)
{
    switch (CharacterMovementState)
    {
        case ERyuMovementState::Sprinting:
        {
            CharacterState = ERyuCharacterState::JumpBackwardFast;
            break;
        }
        case ERyuMovementState::Running:
        case ERyuMovementState::Standing:
        {
            CharacterState = ERyuCharacterState::JumpBackward;
            break;
        }
    }

    // Set JumpGraphics/Sound or other Asset related stuff / fire a event for the engine / although this is controlled by the UnrealPaperZDStateMachine (2DAssetChanges)
}

void URyuCharacterJumpBackwardState::Exit(ARyuBaseCharacter* Character)
{
}

void URyuCharacterJumpBackwardState::SetCharacterMovement(ERyuMovementState JumpMovementState)
{
	CharacterMovementState = JumpMovementState;
}
