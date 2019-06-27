// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterClimbState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterJumpBackwardState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterClimbState::URyuCharacterClimbState()
{
}

IRyuCharacterState* URyuCharacterClimbState::HandleInput(ARyuBaseCharacter* Character,
                                                         const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressJump:
            return NewObject<URyuCharacterJumpBackwardState>();
        default:
            return nullptr;
            break;
    }
}

void URyuCharacterClimbState::Update(ARyuBaseCharacter* Character)
{
    // Super::Update(Character);
}

void URyuCharacterClimbState::Exit(ARyuBaseCharacter* Character)
{
}
