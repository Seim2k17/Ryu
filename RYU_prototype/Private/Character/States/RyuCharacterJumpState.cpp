// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpState.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
//#include "RyuCharacterJumpEndState.h"
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
            return this;
            break;
        }

        case ERyuInputState::InputEndJump:
        {
            //CharacterState = ERyuCharacterState::JumpEnd;
            return NewObject<URyuCharacterJumpEndState>();
        }

        default:
        {
            return this;
            break;
        }
    }
}

void URyuCharacterJumpState::Update(ARyuBaseCharacter* Character)
{
    if (auto* MainChar = Cast<ARyuMainCharacter>(Character))
    {
        float MoveRightInput = MainChar->GetMoveRightInput();
        MainChar->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveRightInput);

        // TODO JumpeEnd / Jump wuith Velocity < 0 will become Falling and FallingEndstate / Edit the yEd !
        // following lines needs to be moved to a new State (FallingState) because it´s not only jumprelated
        // the JumpEndState therefore will become a FallingEndState !!!
        // Test if char is still n air otherwise change state back to idle
        //if (Character->GetCharacterMovement()->IsFalling() && Character->GetVelocity().Z < 0.0f)
        if (Character->GetVelocity().Z < 0.0f)
        {
            if (bCharacterStartFalling == false)
            {
                Character->SetFallingTimer();
                bCharacterStartFalling = true;
            }

            FHitResult TraceHit = MainChar->GetHitResult();
            UE_LOG(LogRyu, Log,
                   TEXT("Character gets back on the ground -> Back to Idle is triggered with at %s "
                        "with %s"),
                   *TraceHit.ImpactPoint.ToString(), *Character->GetVelocity().ToString());

            // KillVelocity when MoveRightButton isn´t pressed anymore
            if (MainChar->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::Inactive)
            {
                MainChar->GetMovementComponent()->Velocity.X = 0.0f;
            }

            // switch to JumpEndState
            if (TraceHit.ImpactPoint.Z != 0.0f)
            {
                UE_LOG(LogRyu, Log, TEXT("JumpUpdate: ImpactPoinz.Z !=0."));
                bCharacterStartFalling = false;
                Character->ResetFallingTimer();
                // TODO recheck Workflow: with HandleInput(InputEndJump character keeps walking --> backtrack why !
                //CharacterState = ERyuCharacterState::JumpEnd;
                Character->HandleInput(ERyuInputState::InputEndJump);
            }
        }
    }
}

void URyuCharacterJumpState::Enter(ARyuBaseCharacter* Character)
{
    if (auto* MainChar = URyuStaticFunctionLibrary::GetMainChar(Character))
    {
        switch (InputPressed)
        {
            case ERyuInputState::PressJump:
            case ERyuInputState::PressJumpBackward:
            case ERyuInputState::PressJumpForward:
            case ERyuInputState::PressJumpUp:
            {
                UE_LOG(LogRyu, Log, TEXT("VStart: %f; CurV(z): %f"),
                       Character->GetCharacterMovement()->JumpZVelocity,
                       Character->GetCharacterMovement()->Velocity.Z);
                UE_LOG(LogRyu, Log, TEXT("CallFrom: %s."), *this->GetName());
                Character->SaveJumpValues();

                UE_LOG(LogRyu, Log, TEXT("JumpStateEnter: JumpToABPNode called"));
                Character->JumpToAnimInstanceNode(Character->JumpNodeName);
                break;
            }
            default:
                break;
        }
    }
}

URyuCharacterState* URyuCharacterJumpState::InputAnimationEnded(ARyuBaseCharacter* Character)
{
    return this; // Character->GetCharacterState();
}
