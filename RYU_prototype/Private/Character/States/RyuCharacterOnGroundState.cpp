// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterOnGroundState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuLookDirection.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterClimbState.h"
#include "RyuCharacterDuckState.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterJumpBackwardState.h"
#include "RyuCharacterJumpForwardState.h"
#include "RyuCharacterJumpUpwardState.h"
#include "RyuCharacterRunState.h"
#include "RyuCharacterSneakState.h"
#include "RyuCharacterState.h"

URyuCharacterOnGroundState::URyuCharacterOnGroundState()
{
}

// TODO: whats with TwoButtonInput ? Like Jump & Forward
IRyuCharacterState* URyuCharacterOnGroundState::HandleInput(ARyuBaseCharacter* Character,
                                                            const ERyuInputState Input)
{
    ++test;
    UE_LOG(LogRyu, Log, TEXT("i: %d"), test);
    switch (Input)
    {
        case ERyuInputState::PressJumpUp:
        case ERyuInputState::PressJump:
        {
            UE_LOG(LogRyu, Log, TEXT("Jump is pressed"));
            return NewObject<URyuCharacterJumpUpwardState>();
        }
        case ERyuInputState::PressJumpForward:
        {
            return NewObject<URyuCharacterJumpForwardState>();
        }
        case ERyuInputState::PressJumpBackward:
        {
            return NewObject<URyuCharacterJumpBackwardState>();
        }
        case ERyuInputState::PressDown:
        {
            UE_LOG(LogRyu, Log, TEXT("Character is ducking"));
            return NewObject<URyuCharacterDuckState>();
        }
        case ERyuInputState::PressLeft:
        case ERyuInputState::PressRight:
        {
            UE_LOG(LogRyu, Log, TEXT("Character starts walking."));
            return NewObject<URyuCharacterRunState>();
        }
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseRight:
        {
            UE_LOG(LogRyu, Log, TEXT("Character finish walking."));
            return NewObject<URyuCharacterIdleState>();
        }
        case ERyuInputState::PressSneakRight:
        case ERyuInputState::PressSneakLeft:
        {
            // TODOD
            /*

			if (CharacterState->GetState() != ERyuCharacterState::Climb) -> SneakActive -.-
				
			*/

            return NewObject<URyuCharacterSneakState>();
        }
        case ERyuInputState::ReleaseDown:
        {
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

void URyuCharacterOnGroundState::FlipCharacter(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("FlipCharacter is called"));
    ERyuLookDirection LookDirection = Character->GetLookDirection();
    switch (InputPressed)
    {
        case ERyuInputState::PressRight:
        {
            if (LookDirection == ERyuLookDirection::Left)
            {
                Character->FlipCharacter();
            }
            UE_LOG(LogRyu, Log, TEXT("Character is walking Right."));
            break;
        }
        case ERyuInputState::PressLeft:
        {
            if (LookDirection == ERyuLookDirection::Right)
            {
                Character->FlipCharacter();
            }
            UE_LOG(LogRyu, Log, TEXT("Character is walking Left."));
            break;
        }
        default:
            break;
    }
}
