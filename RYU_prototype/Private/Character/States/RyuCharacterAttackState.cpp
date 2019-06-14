// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterAttackState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"

URyuCharacterAttackState::URyuCharacterAttackState()
{
}

IRyuCharacterState* URyuCharacterAttackState::HandleInput(ARyuBaseCharacter* Character,
                                                          const ERyuInputState Input)
{
    // TODO: Check which Attack and return appr. AttackState

    return nullptr;
}

void URyuCharacterAttackState::Update(ARyuBaseCharacter* Character)
{
}
