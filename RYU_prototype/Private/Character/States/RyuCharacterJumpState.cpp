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
            //         case ERyuInputState::PressRight:
            //         case ERyuInputState::PressLeft:
            //         {
            //             return Character->GetCharacterState();
            //         }
        /* causes the PaperZD Crash (exception) ?
        case ERyuInputState::AnimationEnded:
        {
            return InputAnimationEnded(Character); //InputAnimationEnde(Character);
        }
		*/
        case ERyuInputState::InputEndJump:
        {
			// TODO hmm is this exact?
			if (Character->GetJumpStartValues().JumpStartAcceleration.GetAbsMax() > 0.0f)
			{
				UE_LOG(LogRyu, Error, TEXT("CharacterShouldRunFurther."));
				// Needable anymore ?
				Character->SetVelocityAfterJump();
				Character->JumpToAnimInstanceNode(Character->RunNodeName);
				return NewObject<URyuCharacterRunState>();
			}
			else
			{
				UE_LOG(LogRyu, Error, TEXT("CharAnimationEnd."));
				Character->ResetJumpStartValues();
				Character->JumpToAnimInstanceNode(Character->IdleNodeName);
				return NewObject<URyuCharacterIdleState>();
			}
            //old: return NewObject<URyuCharacterJumpEndState>();
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

    if (auto* MainChar = Cast<ARyuMainCharacter>(Character))
    {
        float MoveRightInput = MainChar->GetMoveRightInput();
        // UE_LOG(LogRyu, Log, TEXT("AddMovementInput: %f"), MoveRightInput);
        MainChar->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveRightInput);

		// TODO JumpeEnd / Jump wuith Velocity < 0 will become Falling and FallingEndstate / Edit the yEd !
		// following lines needs to be moved to a new State (FallingState) because it´s not only jumprelated
		// the JumpEndState therefore will become a FallingEndState !!!
        // Test if char is still n air otherwise change state back to idle
        if (Character->GetCharacterMovement()->IsFalling() && Character->GetVelocity().Z < 0.0f)
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

            // KillVelocity when MoveRightButton isn´t pressed
            // hm does not work
            if (MainChar->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::Inactive)
            {
                UE_LOG(LogRyu, Log, TEXT("Velocity needs to be resetted here."));
                MainChar->GetMovementComponent()->Velocity.X = 0.0f;
            }

            // switch to JumpEndState
            if (TraceHit.ImpactPoint.Z != 0.0f)
            {
				bCharacterStartFalling = false;
				Character->ResetFallingTimer();
                Character->HandleInput(ERyuInputState::InputEndJump);
            }
        }
    }
}

void URyuCharacterJumpState::Enter(ARyuBaseCharacter* Character)
{
    // we tick when jumping off ? I think we need to Save a Copy not the Pointer !
    if (FMath::IsNearlyEqual(Character->GetCharacterMovement()->Velocity.Z, 0.0f, 0.1f))
    {
        UE_LOG(LogRyu, Log, TEXT("VStart: %f; CurV(z): %f"),
               Character->GetCharacterMovement()->JumpZVelocity,
               Character->GetCharacterMovement()->Velocity.Z);
        UE_LOG(LogRyu, Log, TEXT("CallFrom: %s."), *this->GetName());
        Character->SaveJumpValues();
    }

    Character->JumpToAnimInstanceNode(Character->JumpNodeName);
}

URyuCharacterState* URyuCharacterJumpState::InputAnimationEnded(ARyuBaseCharacter* Character)
{
    //     if (!Character->GetCharacterMovement()->IsFalling())
    //     {
    //         switch (Character->GetCharacterStateEnum())
    //         {
    //             case ERyuCharacterState::JumpUpward:
    //             case ERyuCharacterState::JumpBackward:
    //             case ERyuCharacterState::JumpForward:
    //             {
    //                 //
    //                 return NewObject<URyuCharacterIdleState>();
    //                 break;
    //             }
    //             case ERyuCharacterState::JumpForwardFast:
    //             {
    //                 if ((Character->FindCurrentInputState(ERyuInputState::PressLeft) == true)
    //                     || (Character->FindCurrentInputState(ERyuInputState::PressRight)) == true)
    //                 {
    //                     return NewObject<URyuCharacterRunState>();
    //                 }
    //                 else
    //                 {
    //                     return NewObject<URyuCharacterIdleState>();
    //                 }
    //
    //                 break;
    //             }
    //
    //             default:
    //             {
    //                 return Character->GetCharacterState();
    //                 break;
    //             }
    //         }
    //     }

    return Character->GetCharacterState();
}
