// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterOnGroundState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterJumpState.h"
#include "RyuCharacterDuckState.h"

URyuCharacterOnGroundState::URyuCharacterOnGroundState()
{
}

IRyuCharacterState* URyuCharacterOnGroundState::HandleInput(ARyuBaseCharacter* Character,
                                                            const ERyuInputState Input)
{
    if (Input == ERyuInputState::PressJump)
    {
        UE_LOG(LogRyu, Log, TEXT("Jump is pressed"));
        // we need to return a JumpStateObject
        return NewObject<URyuCharacterJumpState>();
    }

    if (Input == ERyuInputState::PressDown)
    {
        UE_LOG(LogRyu, Log, TEXT("Character will be ducking"));
        // we need to return a JumpStateObject
        return NewObject<URyuCharacterDuckState>();
    }

    return nullptr;
}

void URyuCharacterOnGroundState::Update(ARyuBaseCharacter* Character)
{
}
