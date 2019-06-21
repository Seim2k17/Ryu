// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterSneakState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterSneakState::URyuCharacterSneakState()
{
}

IRyuCharacterState* URyuCharacterSneakState::HandleInput(ARyuBaseCharacter* Character,
                                                         const ERyuInputState Input)
{
    if (Input == ERyuInputState::ReleaseSneak)
    {
        return NewObject<URyuCharacterIdleState>();
    }
    else
    {
        // only make special call when Input occurs which is not handled in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
        return Super::HandleInput(Character, Input);
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
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterSneakState::Exit(ARyuBaseCharacter* Character)
{
    // TODO: check if transition needs to be checked to the other possible states or if its ok just to test it in the other states in HandelInput
    // Hm i think it´s ok just to set the StateEnum in the EnterMethods of the States (Idle will also do this ...) CharacterState = ERyuCharacterState::Idle;
}
