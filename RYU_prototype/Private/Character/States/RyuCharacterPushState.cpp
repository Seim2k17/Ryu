// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterPushState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterPushState::URyuCharacterPushState()
{
}

IRyuCharacterState* URyuCharacterPushState::HandleInput(ARyuBaseCharacter* Character,
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
}

void URyuCharacterPushState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterPushState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::Push;
	// Set IdleGraphics or other Asset related stuff
}

void URyuCharacterPushState::Exit(ARyuBaseCharacter* Character)
{
}
