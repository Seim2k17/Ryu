// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
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
        case ERyuInputState::StateEnded:
        {
            return NewObject<URyuCharacterIdleState>();
        }

        default:
            // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
            return nullptr;
            break;
    }
}

void URyuCharacterJumpState::Update(ARyuBaseCharacter* Character)
{
    // Test if char is still n air otherwise change state back to idle
    if (!Character->GetCharacterMovement()->IsFalling())
    {
        UE_LOG(LogRyu, Log, TEXT("Character gets back on the ground -> Back to Idle is triggered.");
		Character->HandleInput(ERyuInputState::StateEnded);
    }
}
