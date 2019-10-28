// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterOpenState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterOpenState::URyuCharacterOpenState()
{
}

URyuCharacterState* URyuCharacterOpenState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
    if (Input == ERyuInputState::ReleaseDown)
    {
        UE_LOG(LogRyu, Log, TEXT("Character stands up."));
        // we need to return a JumpStateObject
        return NewObject<URyuCharacterIdleState>();
    }
    else
    {
        // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
        return Super::HandleInput(Character, Input);
    }
}

void URyuCharacterOpenState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterOpenState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::Open;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterOpenState::Exit(ARyuBaseCharacter* Character)
{
}
