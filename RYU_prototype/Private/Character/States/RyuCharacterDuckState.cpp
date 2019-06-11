// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterDuckState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterDuckState::URyuCharacterDuckState()
{
}

IRyuCharacterState* URyuCharacterDuckState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
//     if (Input == ERyuInputState::ReleaseDown)
//     {
//         UE_LOG(LogRyu, Log, TEXT("Character stands up."));
//         // we need to return a JumpStateObject
//         return NewObject<URyuCharacterIdleState>();
//     }
//     else
//     {
        // only make special call when Input occurs which is not in the Baseclass, otherwise we don�t need to handle Input, just walk up in the hierarchy
        return Super::HandleInput(Character, Input);
//     }
// 
// 	return nullptr;
}

void URyuCharacterDuckState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterDuckState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::Duck;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterDuckState::Exit(ARyuBaseCharacter* Character)
{
}