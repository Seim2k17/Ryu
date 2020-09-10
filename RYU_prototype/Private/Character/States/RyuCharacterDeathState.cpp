// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterDeathState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterDeathState::URyuCharacterDeathState()
{
}

URyuCharacterState* URyuCharacterDeathState::HandleInput(ARyuBaseCharacter* Character,
	const ERyuInputState Input)
{
		return Super::HandleInput(Character, Input);
}

void URyuCharacterDeathState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterDeathState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::Death;
	Character->SetJumpAllowed(false);
}

void URyuCharacterDeathState::Exit(ARyuBaseCharacter* Character)
{
}
