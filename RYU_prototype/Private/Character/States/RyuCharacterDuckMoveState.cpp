// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterDuckMoveState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterDuckState.h"
#include "RyuCharacterIdleState.h"

URyuCharacterDuckMoveState::URyuCharacterDuckMoveState()
{
}

URyuCharacterState* URyuCharacterDuckMoveState::HandleInput(ARyuBaseCharacter* Character,
                                                            const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressUp:
        {
            UE_LOG(LogRyu, Log, TEXT("Character stands up."));
            return NewObject<URyuCharacterIdleState>();
            break;
        }
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseRight:
        {
            return NewObject<URyuCharacterDuckState>();
            break;
        }
        default:
        {
            return Super::HandleInput(Character, Input);
            break;
        }
    }
}

void URyuCharacterDuckMoveState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterDuckMoveState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::DuckMove;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterDuckMoveState::Exit(ARyuBaseCharacter* Character)
{
}
