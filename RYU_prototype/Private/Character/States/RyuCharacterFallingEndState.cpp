// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterFallingEndState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMovementState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuMainCharacter.h"

URyuCharacterFallingEndState::URyuCharacterFallingEndState()
{
}

URyuCharacterState* URyuCharacterFallingEndState::HandleInput(ARyuBaseCharacter* Character,
                                                              const ERyuInputState Input)
{
    switch (Input)
    {
            // when comment animationendline: character stands after jumping
        case ERyuInputState::AnimationEnded:
        {
            UE_LOG(LogRyu, Error, TEXT("FallingEndState: CharAnimationEnd."));
            // character jumped off and is still in Progress of running
            // 		if (Character->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::Inactive)
            // 		{
            //Character->ResetJumpStartValues();
            UE_LOG(LogRyu, Error, TEXT("FallingEndState: CharacterStops."));
            Character->JumpToAnimInstanceNode(Character->IdleNodeName);
            return NewObject<URyuCharacterIdleState>();
            break;
            //}
        }
        default:
			return nullptr;
            break;
    }
}

void URyuCharacterFallingEndState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterFallingEndState::Enter(ARyuBaseCharacter* Character)
{
    Character->ResetFallingTimer();
	// how to differ here ?
    CharacterState = ERyuCharacterState::FallingNormalEnd;
	//CharacterState = ERyuCharacterState::FallingDeepEnd;
}

void URyuCharacterFallingEndState::Exit(ARyuBaseCharacter* Character)
{
}
