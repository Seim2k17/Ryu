// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterFallingEndState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMovementState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuMainCharacter.h"

URyuCharacterFallingEndState::URyuCharacterFallingEndState()
{
}

URyuCharacterState* URyuCharacterFallingEndState::HandleInput(ARyuBaseCharacter* Character,
                                                              const ERyuInputState Input)
{
	return nullptr;
}

void URyuCharacterFallingEndState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterFallingEndState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::FallingNormalEnd;
}

void URyuCharacterFallingEndState::Exit(ARyuBaseCharacter* Character)
{
}
