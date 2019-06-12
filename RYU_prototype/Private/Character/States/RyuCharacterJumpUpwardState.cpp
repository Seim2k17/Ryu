// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpUpwardState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterJumpUpwardState::URyuCharacterJumpUpwardState()
{
}

IRyuCharacterState* URyuCharacterJumpUpwardState::HandleInput(ARyuBaseCharacter* Character,
	const ERyuInputState Input)
{
	// only make special call when Input occurs which is not in the Baseclass, otherwise we don�t need to handle Input, just walk up in the hierarchy
	return Super::HandleInput(Character, Input);
}

void URyuCharacterJumpUpwardState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterJumpUpwardState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::JumpUpward;
	// Set JumpGraphics/Sound or other Asset related stuff / fire a event for the engine / although this is controlled by the UnrealPaperZDStateMachine (2DAssetChanges)
}

void URyuCharacterJumpUpwardState::Exit(ARyuBaseCharacter* Character)
{
}
