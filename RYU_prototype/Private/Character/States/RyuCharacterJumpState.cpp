// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterJumpState::URyuCharacterJumpState()
{
}

IRyuCharacterState* URyuCharacterJumpState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
	switch (Input)
	{
	case ERyuInputState::PressJump:
		
	{
		// double-Jump / Set Jump-Count or something
		return nullptr;
		break;
	}

	default:
		// only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
		return Super::HandleInput(Character, Input);
		break;
	}
}

void URyuCharacterJumpState::Update(ARyuBaseCharacter* Character)
{
}
