// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpState.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterRunState.h"

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
            return Character->GetCharacterState();
            break;
        }
        case ERyuInputState::AnimationEnded:
        {
            return InputAnimationEnded(Character); //InputAnimationEnde(Character);
        }

        default:
        {
            return Character->GetCharacterState();
            break;
        }
    }
}

void URyuCharacterJumpState::Update(ARyuBaseCharacter* Character)
{
    // Test if char is still n air otherwise change state back to idle
    if (!Character->GetCharacterMovement()->IsFalling())
    {
        UE_LOG(LogRyu, Log,
               TEXT("Character gets back on the ground -> Back to Idle is triggered."));
        // Character->HandleInput(ERyuInputState::StateEnded);
    }
}

void URyuCharacterJumpState::Enter(ARyuBaseCharacter* Character)
{
    Character->JumpToAnimInstanceNode(Character->JumpNodeName);
}

IRyuCharacterState* URyuCharacterJumpState::InputAnimationEnded(ARyuBaseCharacter* Character)
{
    switch (Character->GetCharacterStateEnum())
    {
        case ERyuCharacterState::JumpUpward:
        case ERyuCharacterState::JumpBackward:
        case ERyuCharacterState::JumpForward:
        {
            //
            return NewObject<URyuCharacterIdleState>();
            break;
        }
        case ERyuCharacterState::JumpForwardFast:
        {
            if ((Character->FindCurrentInputState(ERyuInputState::PressLeft) == true)
                || (Character->FindCurrentInputState(ERyuInputState::PressRight)) == true)
            {
                return NewObject<URyuCharacterRunState>();
            }
            else
            {
                return NewObject<URyuCharacterIdleState>();
            }

            break;
        }

        default:
        {
            return Character->GetCharacterState();
            break;
        }
    }
}
