// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterCombatState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterCombatState::URyuCharacterCombatState()
{
}

IRyuCharacterState* URyuCharacterCombatState::HandleInput(ARyuBaseCharacter* Character,
	const ERyuInputState Input)
{
// 	if (Input == ERyuInputState::ReleaseDown)
// 	{
// 		UE_LOG(LogRyu, Log, TEXT("Character stands up."));
// 		// we need to return a JumpStateObject
// 		return NewObject<URyuCharacterIdleState>();
// 	}
// 	else
// 	{
		// only make special call when Input occurs which is not in the Baseclass, otherwise we don�t need to handle Input, just walk up in the hierarchy
		return Super::HandleInput(Character, Input);
// 	}
// 
// 	return nullptr;
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
