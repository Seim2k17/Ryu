// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterRunState.h"
#include "Enums/ERyuInputState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuMainCharacter.h"

URyuCharacterRunState::URyuCharacterRunState()
{
}

IRyuCharacterState* URyuCharacterRunState::HandleInput(ARyuBaseCharacter* Character,
                                                       const ERyuInputState Input)
{
    if (Input == ERyuInputState::PressJump) //|| JumpForward ?
    {
		// if other direction is pressed
		//return NewObject<URyuCharacterJumpBackwardState>();
        return NewObject<URyuCharacterJumpForwardState>(false);
    }
    else
    {
        // only make special call when Input occurs which is not handled in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
        return Super::HandleInput(Character, Input);
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
