// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterOnGroundState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuLookDirection.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterClimbState.h"
#include "RyuCharacterDuckState.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterJumpBackwardState.h"
#include "RyuCharacterJumpForwardState.h"
#include "RyuCharacterJumpUpwardState.h"
#include "RyuCharacterRunState.h"
#include "RyuCharacterSneakState.h"
#include "RyuCharacterState.h"

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
    UE_LOG(LogRyu, Log, TEXT("OnGround(SuperCall) | Update: CharacterSpeed: %s "),
           *Character->GetCharacterMovement()->Velocity.ToString());
}

void URyuCharacterOnGroundState::FlipCharacter(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("FlipCharacter is called"));
    ERyuLookDirection LookDirection = Character->GetLookDirection();
    switch (InputPressed)
    {
        case ERyuInputState::PressRight:
        {
            if (LookDirection == ERyuLookDirection::Left)
            {
                Character->FlipCharacter();
            }
            UE_LOG(LogRyu, Log, TEXT("Character is walking Right."));
            break;
        }
        case ERyuInputState::PressLeft:
        {
            if (LookDirection == ERyuLookDirection::Right)
            {
                Character->FlipCharacter();
            }
            UE_LOG(LogRyu, Log, TEXT("Character is walking Left."));
            break;
        }
        default:
            UE_LOG(LogRyu, Log,
                   TEXT("Hm. InputState is neither Left or Right. Its: %s at speed: %s"),
                   *URyuStaticFunctionLibrary::InputStateToString(InputPressed),
                   *Character->GetCharacterMovement()->Velocity.ToString());
            break;
    }
}

void URyuCharacterOnGroundState::Enter(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("OnGround(SuperCall) | Enter: CharacterSpeed: %s "),
           *Character->GetCharacterMovement()->Velocity.ToString());
}
