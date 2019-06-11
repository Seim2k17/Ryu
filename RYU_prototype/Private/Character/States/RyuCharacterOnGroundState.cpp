// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterJumpState.h"
#include "RyuCharacterDuckState.h"
#include "RyuCharacterClimbState.h"

URyuCharacterOnGroundState::URyuCharacterOnGroundState()
{
}

IRyuCharacterState* URyuCharacterOnGroundState::HandleInput(ARyuBaseCharacter* Character,
                                                            const ERyuInputState Input)
{
    if (Input == ERyuInputState::PressJump)
    {
        UE_LOG(LogRyu, Log, TEXT("Jump is pressed"));
        // we need to return a JumpStateObject
        return NewObject<URyuCharacterJumpState>();
    }

    if (Input == ERyuInputState::PressDown)
    {
        UE_LOG(LogRyu, Log, TEXT("Character will be ducking"));
        // we need to return a JumpStateObject
        return NewObject<URyuCharacterDuckState>();
    }

	if (Input == ERyuInputState::PressUp)
	{
		// Do we need to check if climbing possible here ?
		UE_LOG(LogRyu, Log, TEXT("Character will be Climbing"));
		// we need to return a JumpStateObject
		return NewObject<URyuCharacterClimbState>();
	}

    return nullptr;
}

void URyuCharacterOnGroundState::Update(ARyuBaseCharacter* Character)
{
}


// ERyuCharacterState URyuCharacterOnGroundState::GetState_Implementation()
// {
// 	return CharacterState;
// }
