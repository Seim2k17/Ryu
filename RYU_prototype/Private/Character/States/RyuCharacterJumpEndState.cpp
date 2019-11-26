// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpEndState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterJumpEndState.h"
#include "RyuCharacterRunState.h"
#include "RyuMainCharacter.h"

URyuCharacterJumpEndState::URyuCharacterJumpEndState()
{
}

URyuCharacterState* URyuCharacterJumpEndState::HandleInput(ARyuBaseCharacter* Character,
                                                           const ERyuInputState Input)
{
    switch (Input)
    {
        // when comment animationendline: character stands after jumping
        case ERyuInputState::AnimationEnded:
        case ERyuInputState::InputEndJump:
        {
            /*
			if (Character->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::PressLeftAxisKey)

			{
				InputPressed = ERyuInputState::PressLeft;
			}

			if (Character->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::PressRightAxisKey)
			{
				InputPressed = ERyuInputState::PressRight;
			}

			*/

            // character jumped off and is still in Progress of running
            if (Character->GetJumpStartValues().JumpStartAcceleration.GetAbsMax() > 0.0f)
            {
                UE_LOG(LogRyu, Error, TEXT("CharacterShouldRunFurther."));
                Character->SetVelocityAfterJump();
                return NewObject<URyuCharacterRunState>();
            }
            else
            {
                UE_LOG(LogRyu, Error, TEXT("CharAnimationEnd."));
                Character->ResetJumpStartValues();
                return NewObject<URyuCharacterIdleState>();
            }
        }
    }
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
    return Super::HandleInput(Character, Input);
    //	return this;
}

void URyuCharacterJumpEndState::Update(ARyuBaseCharacter* Character)
{
    Super::Update(Character);
}

void URyuCharacterJumpEndState::Enter(ARyuBaseCharacter* Character)
{
    Super::Enter(Character);
    CharacterState = ERyuCharacterState::JumpEnd;

    // start timer for fallback (make sure switching at the end of the JumpEnd back to IDLE)
    Character->SetEndJumpTimer();
}

void URyuCharacterJumpEndState::Exit(ARyuBaseCharacter* Character)
{
    Super::Exit(Character);
    //CharacterState = ERyuCharacterState::None;
    UE_LOG(LogRyu, Log, TEXT("SetInputpressedVar"));
    if (Character->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::PressLeftAxisKey)

    {
        InputPressed = ERyuInputState::PressLeft;
    }

    if (Character->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::PressRightAxisKey)
    {
        InputPressed = ERyuInputState::PressRight;
    }
}
