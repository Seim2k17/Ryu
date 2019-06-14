// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterCombatState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterAttackState.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterCombatState::URyuCharacterCombatState()
{
}

IRyuCharacterState* URyuCharacterCombatState::HandleInput(ARyuBaseCharacter* Character,
                                                          const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressAttack:
        {
            return NewObject<URyuCharacterAttackState>();
        }
    }
    return Super::HandleInput(Character, Input);
}

void URyuCharacterCombatState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterCombatState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::Combat;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterCombatState::Exit(ARyuBaseCharacter* Character)
{
}
