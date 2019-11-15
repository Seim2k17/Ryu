// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpState.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterJumpEndState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterRunState.h"

URyuCharacterJumpState::URyuCharacterJumpState()
{
}

URyuCharacterState* URyuCharacterJumpState::HandleInput(ARyuBaseCharacter* Character,
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
        /* causes the PaperZD Crash (exception) ?
        case ERyuInputState::AnimationEnded:
        {
            return InputAnimationEnded(Character); //InputAnimationEnde(Character);
        }
		*/
        case ERyuInputState::InputEndJump:
        {
            // only tmp we need a new state for EndJump to make all consistent calls etc. HMPF
            return NewObject<URyuCharacterJumpEndState>();
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
    // didn´see anything from the LineTrace ! why ????

    // Test if char is still n air otherwise change state back to idle
    if (Character->GetCharacterMovement()->IsFalling() && Character->GetVelocity().Z < 0.0f)
    {
        if (auto* MainChar = Cast<ARyuMainCharacter>(Character))
        {
            FHitResult TraceHit = MainChar->GetHitResult();
            UE_LOG(LogRyu, Log,
                   TEXT(
                       "Character gets back on the ground -> Back to Idle is triggered with at %s"),
                   *TraceHit.ImpactPoint.ToString());

            if (TraceHit.ImpactPoint.Z != 0.0f)
            {
                Character->HandleInput(ERyuInputState::InputEndJump);
            }
        }
    }
}

void URyuCharacterJumpState::Enter(ARyuBaseCharacter* Character)
{
    Character->JumpToAnimInstanceNode(Character->JumpNodeName);
}

URyuCharacterState* URyuCharacterJumpState::InputAnimationEnded(ARyuBaseCharacter* Character)
{
    if (!Character->GetCharacterMovement()->IsFalling())
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

    return Character->GetCharacterState();
}
