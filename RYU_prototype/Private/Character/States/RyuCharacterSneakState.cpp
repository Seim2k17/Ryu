// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterSneakState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMovementState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterDuckState.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterSneakState::URyuCharacterSneakState()
{
}

IRyuCharacterState* URyuCharacterSneakState::HandleInput(ARyuBaseCharacter* Character,
                                                         const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressDown:
        {
            return NewObject<URyuCharacterDuckMoveState>();
            break;
        }
        case ERyuInputState::ReleaseSneak:
        {
            return NewObject<URyuCharacterIdleState>();
            break;
        }
        default:
            return Super::HandleInput(Character, Input);
            break;
    }

    return nullptr;
}

void URyuCharacterSneakState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterSneakState::Enter(ARyuBaseCharacter* Character)
{
    Super::FlipCharacter(Character);

    CharacterState = ERyuCharacterState::Sneak;
	Character->SetCharacterMovementState(ERyuMovementState::Sneaking);
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterSneakState::Exit(ARyuBaseCharacter* Character)
{
    // TODO: check if transition needs to be checked to the other possible states or if its ok just to test it in the other states in HandelInput
    // Hm i think it´s ok just to set the StateEnum in the EnterMethods of the States (Idle will also do this ...) CharacterState = ERyuCharacterState::Idle;
}
