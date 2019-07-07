// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterRunState.h"
#include "Enums/ERyuInputState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterRollState.h"
#include "RyuCharacterSneakState.h"
#include "RyuCharacterSprintState.h"
#include "RyuMainCharacter.h"

URyuCharacterRunState::URyuCharacterRunState()
{
}

IRyuCharacterState* URyuCharacterRunState::HandleInput(ARyuBaseCharacter* Character,
                                                       const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressDown:
        {
            return NewObject<URyuCharacterRollState>();
            break;
        }
        case ERyuInputState::PressJump:
        {
            return NewObject<URyuCharacterJumpForwardState>(false);
            break;
        }
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseLeft:
        {
            return NewObject<URyuCharacterIdleState>();
            break;
        }
        case ERyuInputState::PressSneakLeft:
        case ERyuInputState::PressSneakRight:
        {
            return NewObject<URyuCharacterSneakState>();
            break;
        }
        case ERyuInputState::PressSprintLeft:
        case ERyuInputState::PressSprintRight:
        {
            if (Character->GetCharacterStatus(ERyuCharacterStatus::Stamina > 0.0f))
            {
                return NewObject<URyuCharacterSprintState>();
                break;
            }
        }
        default:
            return Super::HandleInput(Character, Input);
            break;
    }

    return nullptr;
}

void URyuCharacterRunState::Update(ARyuBaseCharacter* Character)
{
    /* TODO check: Old Movement when Turning ! needable ?
	if (PlayerMovement == EPlayerMovement::STARTTURNRUN)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), -0.1f * Val);
		UE_LOG(LogTemp, Log, TEXT("MoveRight(): Turn while Running %s:"),
			   *FString::SanitizeFloat((-0.1 * Val)));
	}

	*/
    if (auto* MainChar = Cast<ARyuMainCharacter>(Character))
    {
        float MoveRightInput = MainChar->GetMoveRightInput();
        MainChar->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveRightInput);
    }
}

void URyuCharacterRunState::Enter(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("InputState: %s"),
           *URyuStaticFunctionLibrary::InputStateToString(InputPressed));
    Super::FlipCharacter(Character);

    CharacterState = ERyuCharacterState::Run;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterRunState::Exit(ARyuBaseCharacter* Character)
{
    Super::Exit(Character);
}
