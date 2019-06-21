// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterClimbState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterClimbState::URyuCharacterClimbState()
{
}

IRyuCharacterState* URyuCharacterClimbState::HandleInput(ARyuBaseCharacter* Character,
                                                         const ERyuInputState Input)
{
    return Super::HandleInput(Character, Input);
}

void URyuCharacterClimbState::Update(ARyuBaseCharacter* Character)
{
    // Super::Update(Character);
}

void URyuCharacterClimbState::Exit(ARyuBaseCharacter* Character)
{
}
