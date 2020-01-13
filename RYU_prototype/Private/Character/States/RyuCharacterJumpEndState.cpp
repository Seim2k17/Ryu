// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpEndState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMoveRightAxisInputState.h"
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
        {
            UE_LOG(LogRyu, Error, TEXT("JumpEndState: CharAnimationEnd."));
            // character jumped off and is still in Progress of running
            if (Character->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::Inactive)
            {
                //Character->ResetJumpStartValues();
                UE_LOG(LogRyu, Error, TEXT("JumpEndState: CharacterStops."));
                Character->JumpToAnimInstanceNode(Character->IdleNodeName);
                return NewObject<URyuCharacterIdleState>();
                break;
            }
            else
            {
                UE_LOG(LogRyu, Error, TEXT("JumpEndState: CharacterShouldRunFurther."));
                //Character->SetVelocityAfterJump();
                Character->JumpToAnimInstanceNode(Character->RunNodeName);
                return NewObject<URyuCharacterRunState>();
                break;
            }
        }
        default:
        {
            // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
            return this;
            //return Super::HandleInput(Character, Input);
            break;
            //
        }
    }
}

void URyuCharacterJumpEndState::Update(ARyuBaseCharacter* Character)
{
    //Super::Update(Character);

    if (auto* MainChar = Cast<ARyuMainCharacter>(Character))
    {
        float MoveRightInput = MainChar->GetMoveRightInput();
        UE_LOG(LogRyu, Log, TEXT("RunState: AddMovementInput: %f"), MoveRightInput);
        MainChar->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveRightInput);

        UE_LOG(LogRyu, Log, TEXT("UpdateJumpEndAState: Character runs with %s "),
               *Character->GetVelocity().ToString());
    }
}

void URyuCharacterJumpEndState::Enter(ARyuBaseCharacter* Character)
{
    Super::Enter(Character);
    CharacterState = ERyuCharacterState::JumpEnd;

    // start timer for fallback (make sure switching at the end of the JumpEnd back to IDLE)
    //Character->SetEndJumpTimer();
}

void URyuCharacterJumpEndState::Exit(ARyuBaseCharacter* Character)
{
    //Super::Exit(Character);
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

    if (auto* MoveComp = Character->GetRyuCharacterMovement())
    {
        MoveComp->EndJumpTime = UGameplayStatics::GetTimeSeconds(Character);
        MoveComp->OverallJumpTime = MoveComp->EndJumpTime - MoveComp->StartJumpTime;
        UE_LOG(LogRyu, Log, TEXT("JumpEndState:Exit(): OverallJumpTime: %f s"),
               MoveComp->OverallJumpTime);
    }
}
