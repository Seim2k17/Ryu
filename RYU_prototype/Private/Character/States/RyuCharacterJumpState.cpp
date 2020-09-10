// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpState.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuMainCharacter.h"
//#include "RyuCharacterJumpEndState.h"
#include "RyuCharacterFallingState.h"
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
			break;
        }
        case ERyuInputState::InputFalling:
        {
            //CharacterState = ERyuCharacterState::JumpEnd;
            return NewObject<URyuCharacterFallingState>();
			break;
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
        if ((Character->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::PressLeftAxisKey)
            || (Character->GetMoveRightAxisState()
                == ERyuMoveRightAxisInputState::PressRightAxisKey))
        {
            UE_LOG(LogTemp, Log, TEXT("press left || right from JumpUpdate"));
            // TODO: here we need to check if diorection changes then flip .... (and only once ...)
            this->FlipCharacter(Character);
            //InputPressed = ERyuInputState::PressLeft;
        }

        float MoveRightInput = MainChar->GetMoveRightInput();
		MainChar->AddMovementInput(FVector(1.0f, 0.0f, 0.0f),
			MoveRightInput / MainChar->GetFallingMoveRightMultiplier());
        //MainChar->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveRightInput);

		
        // TODO: !!! JumpeEnd / Jump wuith Velocity < 0 will become Falling and FallingEndstate / Edit the yEd !
        // following lines needs to be moved to a new State (FallingState) because it´s not only jumprelated
        // Test if char is still n air otherwise change state back to idle
        auto FallDownSpeed = Character->GetVelocity();
        if (FallDownSpeed.Z < 0.0f)
        {
            if (bCharacterStartFalling == false)
            {
                Character->SetFallingTimer();
                bCharacterStartFalling = true;
            }

            FHitResult TraceHit = MainChar->GetHitResult();

            // KillVelocity when MoveRightButton isn´t pressed anymore
            if (MainChar->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::Inactive)
            {
                MainChar->GetMovementComponent()->Velocity.X = 0.0f;
            }

            if (FallDownSpeed.Z < Character->GetFallVelocityZFromJump())
            {
                // TODO: we need a possibility to change the Input WITHOUT using an Input ! do we really ?
                Character->HandleInput(ERyuInputState::InputFalling);
            }
            else
            {
                // switch to JumpEndState
                if (TraceHit.bBlockingHit)
                {
                    UE_LOG(LogRyu, Log,
                           TEXT("Character gets back on the ground -> Back to Idle is triggered "
                                "with at %s "
                                "with %s"),
                           *TraceHit.ImpactPoint.ToString(), *Character->GetVelocity().ToString());

                    UE_LOG(LogRyu, Log, TEXT("JumpUpdate: ImpactPoinz: %s"),
                           *TraceHit.ImpactPoint.ToString());

                    // TODO recheck Workflow: with HandleInput(InputEndJump character keeps walking --> backtrack why !
                    //CharacterState = ERyuCharacterState::JumpEnd;
                    Character->HandleInput(ERyuInputState::InputEndJump);
                }
            }
        }
		
		
    }
}

void URyuCharacterJumpState::Enter(ARyuBaseCharacter* Character)
{
    bCharacterStartFalling = false;
    if (auto* MainChar = URyuStaticFunctionLibrary::GetMainChar(Character))
    {
        if (auto* MoveComp = MainChar->GetRyuCharacterMovement())
        {
            // TODO Call correct ?
            MoveComp->StartJumpTime = UGameplayStatics::GetTimeSeconds(Character);
        }

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

void URyuCharacterJumpState::Exit(ARyuBaseCharacter* Character)
{
    bCharacterStartFalling = false;
    Character->ResetFallingTimer();
    // TODO: check if this is a source of error:
	InputPressed = ERyuInputState::None;
    //this->ConditionalBeginDestroy();
    //this = nullptr;
}

URyuCharacterState* URyuCharacterJumpState::InputAnimationEnded(ARyuBaseCharacter* Character)
{
    return this; // Character->GetCharacterState();
}

void URyuCharacterJumpState::FlipCharacter(ARyuBaseCharacter* Character)
{
    Super::FlipCharacter(Character);
}
