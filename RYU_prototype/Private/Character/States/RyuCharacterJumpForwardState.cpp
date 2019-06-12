// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpForwardState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterJumpForwardState::URyuCharacterJumpForwardState()
{
}

IRyuCharacterState* URyuCharacterJumpForwardState::HandleInput(ARyuBaseCharacter* Character,
	const ERyuInputState Input)
{
	// only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
	return Super::HandleInput(Character, Input);
}

void URyuCharacterJumpForwardState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterJumpForwardState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::JumpForward;
	// Set JumpGraphics/Sound or other Asset related stuff / fire a event for the engine / although this is controlled by the UnrealPaperZDStateMachine (2DAssetChanges)
}

void URyuCharacterJumpForwardState::Exit(ARyuBaseCharacter* Character)
{
}
