// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterMoveObjectState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"

URyuCharacterMoveObjectState::URyuCharacterMoveObjectState()
{
}

IRyuCharacterState* URyuCharacterMoveObjectState::HandleInput(ARyuBaseCharacter* Character,
                                                              const ERyuInputState Input)
{
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
    return Super::HandleInput(Character, Input);
}

void URyuCharacterMoveObjectState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterMoveObjectState::Exit(ARyuBaseCharacter* Character)
{
}
