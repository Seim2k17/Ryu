// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterInAirState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterJumpState.h"

URyuCharacterInAirState::URyuCharacterInAirState()
{
}

IRyuCharacterState* URyuCharacterInAirState::HandleInput(ARyuBaseCharacter* Character,
                                                         const ERyuInputState Input)
{
//     if (Input == ERyuInputState::PressJump)
//     {
//         UE_LOG(LogRyu, Log, TEXT("Jump in Air is pressed"));
//         // we need to return a JumpStateObject
//         return NewObject<URyuCharacterJumpState>();
//     }

    return nullptr;
}

void URyuCharacterInAirState::Update(ARyuBaseCharacter* Character)
{
}