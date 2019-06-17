// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterOnGroundState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuLookDirection.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterClimbState.h"
#include "RyuCharacterDuckState.h"
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
            ERyuLookDirection LookDirection = Character->GetLookDirection();
            if (Input == ERyuInputState::PressRight)
            {
                if (LookDirection == ERyuLookDirection::Left)
                {
                    Character->FlipCharacter();
                }
                UE_LOG(LogRyu, Log, TEXT("Character is walking Right."));
            }
            else
            {
                if (LookDirection == ERyuLookDirection::Right)
                {
                    Character->FlipCharacter();
                }
                UE_LOG(LogRyu, Log, TEXT("Character is walking Left."));
            }

            return NewObject<URyuCharacterRunState>();
        }
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseRight:
        {
            UE_LOG(LogRyu, Log, TEXT("Character finished walking."));
            return NewObject<URyuCharacterIdleState>();
        }
        case ERyuInputState::PressSneakRight:
        case ERyuInputState::PressSneakLeft:
        {
            ERyuLookDirection LookDirection = Character->GetLookDirection();
            if (Input == ERyuInputState::PressSneakRight)
            {
                if (LookDirection == ERyuLookDirection::Left)
                {
                    Character->FlipCharacter();
                }
                UE_LOG(LogRyu, Log, TEXT("Character is walking Right."));
            }
            else
            {
                if (LookDirection == ERyuLookDirection::Right)
                {
                    Character->FlipCharacter();
                }
                UE_LOG(LogRyu, Log, TEXT("Character is walking Left."));
            }

            return NewObject<URyuCharacterSneakState>();
        }

        default:
            break;
    }

    return nullptr;
}

void URyuCharacterOnGroundState::Update(ARyuBaseCharacter* Character)
{
}
