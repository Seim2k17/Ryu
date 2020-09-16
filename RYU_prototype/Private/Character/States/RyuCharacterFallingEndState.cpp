// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterFallingEndState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMovementState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterDeathState.h"
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
            //UE_LOG(LogRyu, Error, TEXT("FallingEndState: CharAnimationEnd."));
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
        case ERyuInputState::InputFallingDeath:
        {
            return NewObject<URyuCharacterDeathState>();
        }
        default:
            return nullptr;
            break;
    }
}

void URyuCharacterFallingEndState::Update(ARyuBaseCharacter* Character)
{
    // how to differ here ?
    switch (Character->GetLastCharacterState())
    {
        case ERyuCharacterState::FallingNormal:
        {
            CharacterState = ERyuCharacterState::FallingNormalEnd;
            //Character->SetLastCharacterState(ERyuCharacterState::FallingNormalEnd);
            break;
        }
        case ERyuCharacterState::FallingDeep:
        {
            CharacterState = ERyuCharacterState::FallingDeepEnd;
            //Character->SetLastCharacterState(ERyuCharacterState::FallingDeepEnd);
            break;
        }

        case ERyuCharacterState::FallingDeath:
        {
            Character->HandleInput(ERyuInputState::InputFallingDeath);
            //Character->SetLastCharacterState(ERyuCharacterState::FallingDeath);
            break;
        }
        case ERyuCharacterState::Run:
        case ERyuCharacterState::Idle:
        case ERyuCharacterState::JumpEnd:
        case ERyuCharacterState::None:
        {
            //CharacterState = ERyuCharacterState::Death;
            CharacterState = ERyuCharacterState::FallingEnd;
            //Character->SetLastCharacterState(ERyuCharacterState::FallingEnd);
            break;
        }

        default:
            //             UE_LOG(LogRyu, Warning, TEXT("LastCharState : %s"),
            //                    *URyuStaticFunctionLibrary::CharacterStateToString(
            //                        Character->GetLastCharacterState()))
            //CharacterState = ERyuCharacterState::FallingEnd;
            //Character->HandleInput(ERyuInputState::InputEndJump);
            break;
    }
}

void URyuCharacterFallingEndState::Enter(ARyuBaseCharacter* Character)
{
    Character->SetJumpAllowed(false);
    Character->ResetFallingTimer();
}

void URyuCharacterFallingEndState::Exit(ARyuBaseCharacter* Character)
{
    Super::Exit(Character);
    Character->SetJumpAllowed(true);
    Character->SetLastCharacterState(ERyuCharacterState::None);
}
