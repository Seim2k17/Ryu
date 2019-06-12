// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterOnGroundState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterClimbState.h"
#include "RyuCharacterDuckState.h"
#include "RyuCharacterJumpState.h"
#include "RyuCharacterRunState.h"
#include "RyuCharacterState.h"

URyuCharacterOnGroundState::URyuCharacterOnGroundState()
{
}

IRyuCharacterState* URyuCharacterOnGroundState::HandleInput(ARyuBaseCharacter* Character,
                                                            const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressJump: {
            UE_LOG(LogRyu, Log, TEXT("Jump is pressed"));
            // we need to return a JumpStateObject
            return NewObject<URyuCharacterJumpState>();
        }
        case ERyuInputState::PressDown: {
            UE_LOG(LogRyu, Log, TEXT("Character will be ducking"));
            // we need to return a JumpStateObject
            return NewObject<URyuCharacterDuckState>();
        }
        case ERyuInputState::PressUp: {
            // Do we need to check if climbing possible here ?
            UE_LOG(LogRyu, Log, TEXT("Character will be Climbing"));
            // we need to return a JumpStateObject
            return NewObject<URyuCharacterClimbState>();
        }
        case ERyuInputState::PressLeft:
        case ERyuInputState::PressRight: {
            if (Input == ERyuInputState::PressRight)
            {
                UE_LOG(LogRyu, Log, TEXT("Character is walking Right."));
            }
            else
            {
                UE_LOG(LogRyu, Log, TEXT("Character is walking Left."));
            }
            return NewObject<URyuCharacterRunState>();
        }
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseRight: {
            UE_LOG(LogRyu, Log, TEXT("Character finished walking."));
            return NewObject<URyuCharacterIdleState>();
        }

        default:
            break;
    }

    return nullptr;
}

void URyuCharacterOnGroundState::Update(ARyuBaseCharacter* Character)
{
}

// ERyuCharacterState URyuCharacterOnGroundState::GetState_Implementation()
// {
// 	return CharacterState;
// }
