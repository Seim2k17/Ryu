// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterIdleState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"

URyuCharacterIdleState::URyuCharacterIdleState()
{
}

IRyuCharacterState* URyuCharacterIdleState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
//     IRyuCharacterState* state = URyuCharacterOnGroundState::HandleInput(Character, Input);
//     return state;

	return Super::HandleInput(Character, Input);
}

void URyuCharacterIdleState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterIdleState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::Idle;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterIdleState::Exit(ARyuBaseCharacter* Character)
{
}
	