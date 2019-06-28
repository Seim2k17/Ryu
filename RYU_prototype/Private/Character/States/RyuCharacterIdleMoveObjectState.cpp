// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterIdleMoveObjectState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterIdleMoveObjectState::URyuCharacterIdleMoveObjectState()
{
}

IRyuCharacterState* URyuCharacterIdleMoveObjectState::HandleInput(ARyuBaseCharacter* Character,
	const ERyuInputState Input)
{
	if (Input == ERyuInputState::PressInteract)
	{
		// we end the interaction with the Object, Transition (EndMoveObjectState) will be handled in the ABP
		return NewObject<URyuCharacterIdleState>();
	}
	else
	{
		// only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
		return Super::HandleInput(Character, Input);
	}

	return nullptr;
}

void URyuCharacterIdleMoveObjectState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterIdleMoveObjectState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::IdleMoveObject;
	// Set IdleGraphics or other Asset related stuff
}

void URyuCharacterIdleMoveObjectState::Exit(ARyuBaseCharacter* Character)
{
}
