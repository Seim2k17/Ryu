// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterOnGroundState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuLookDirection.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterClimbState.h"
#include "RyuCharacterDuckState.h"
#include "RyuCharacterFallingState.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterJumpBackwardState.h"
#include "RyuCharacterJumpForwardState.h"
#include "RyuCharacterJumpUpwardState.h"
#include "RyuCharacterRunState.h"
#include "RyuCharacterSneakState.h"
#include "RyuCharacterState.h"
#include "RyuMainCharacter.h"

URyuCharacterOnGroundState::URyuCharacterOnGroundState()
{
}

// TODO: whats with TwoButtonInput ? Like Jump & Forward; thats should be solved within the CSM-States because we´re e.g. in the running state and change it to Jump, then we trigger the transition ferom running to jump (= far jump)
URyuCharacterState* URyuCharacterOnGroundState::HandleInput(ARyuBaseCharacter* Character,
                                                            const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::AnimationEnded:
        {
            return InputAnimationEnded(Character); //InputAnimationEnde(Character);
            break;
        }
        case ERyuInputState::InputFalling:
        {
            return NewObject<URyuCharacterFallingState>();
            break;
        }
        default:
        {
            // ueberall checken return this (for current state!) , toll dann bewegt sich der dude garnet und die Ani haengt
            // TODO: bug here return Character->GetCharacterState(); // this;
            return nullptr;
            break;
        }
    }

    /** First we define the Action how described in GDD-CSM-Diagram !, then we´ll decide which states can go up to the BaseClasses /
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
            return NewObject<URyuCharacterJumpForwardState>(false);
        }
        case ERyuInputState::PressJumpBackward:
        {
            return NewObject<URyuCharacterJumpBackwardState>(false);
        }
        case ERyuInputState::PressDown:
        {
            UE_LOG(LogRyu, Log, TEXT("Character is ducking"));
            return NewObject<URyuCharacterDuckState>();
        }
        case ERyuInputState::PressLeft:
        case ERyuInputState::PressRight:
        {
            UE_LOG(LogRyu, Log, TEXT("Character starts walking."));
            return NewObject<URyuCharacterRunState>();
        }
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseRight:
        {
            UE_LOG(LogRyu, Log, TEXT("Character finish walking."));
            return NewObject<URyuCharacterIdleState>();
        }
        case ERyuInputState::PressSneakRight:
        case ERyuInputState::PressSneakLeft:
        {
            // TODO
            // if (CharacterState->GetState() != ERyuCharacterState::Climb) -> SneakActive -.-
				
			

            return NewObject<URyuCharacterSneakState>();
        }
        case ERyuInputState::ReleaseDown:
        {
            return NewObject<URyuCharacterIdleState>();
        }

        default:
            break;
    }

	*/
}

URyuCharacterState* URyuCharacterOnGroundState::InputAnimationEnded(ARyuBaseCharacter* Character)
{
    switch (Character->GetCharacterStateEnum())
    {
        case ERyuCharacterState::Roll:

        {
            UE_LOG(LogRyu, Log, TEXT("RollAniEnded"));
            return this;
            break;
        }
        default:
        {
            return this;
            break;
        }
    }
}

void URyuCharacterOnGroundState::Update(ARyuBaseCharacter* Character)
{
    if (auto MainChar = URyuStaticFunctionLibrary::GetMainChar(Character))
    {
        auto FallDownSpeed = Character->GetVelocity();
        if (FallDownSpeed.Z < 0.0f)
        {
            UE_LOG(LogRyu, Log, TEXT("OnGround(SuperCall) | Update: CharacterSpeed: %s "),
                   *FallDownSpeed.ToString());

            /* neeadable ?
            if (bCharacterStartFalling == false)
            {
                Character->SetFallingTimer();
                bCharacterStartFalling = true;
            }
			*/

            FHitResult TraceHit = MainChar->GetHitResult();

            // KillVelocity when MoveRightButton isn´t pressed anymore
            if (MainChar->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::Inactive)
            {
                MainChar->GetMovementComponent()->Velocity.X = 0.0f;
            }

            //if (FallDownSpeed.Z < Character->GetFallNormalVelocityZ())
			if (FallDownSpeed.Z < 0.0f)
            {
                // TODO: we need a possibility to change the Input WITHOUT using an Input ! do we really ?
                Character->HandleInput(ERyuInputState::InputFalling);
            }
        }
    }
}

void URyuCharacterOnGroundState::FlipCharacter(ARyuBaseCharacter* Character)
{
    Super::FlipCharacter(Character);
}

void URyuCharacterOnGroundState::Enter(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("OnGround(SuperCall) | Enter: CharacterSpeed: %s "),
           *Character->GetCharacterMovement()->Velocity.ToString());
}
