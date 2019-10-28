// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterDuckState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterDuckMoveState.h"
#include "RyuCharacterIdleState.h"

URyuCharacterDuckState::URyuCharacterDuckState()
{
}

URyuCharacterState* URyuCharacterDuckState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressRight:
        case ERyuInputState::PressLeft:
        {
            return NewObject<URyuCharacterDuckMoveState>();
            break;
        }
        case ERyuInputState::PressUp:
        {
            UE_LOG(LogRyu, Log, TEXT("Character stands up."));
            return NewObject<URyuCharacterIdleState>();
            break;
        }
        default:
        {
            return Super::HandleInput(Character, Input);
            break;
        }
    }
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
