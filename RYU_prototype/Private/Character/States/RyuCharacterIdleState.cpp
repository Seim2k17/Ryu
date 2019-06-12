// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterIdleState.h"
#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterClimbState.h"
#include "RyuCharacterState.h"

URyuCharacterIdleState::URyuCharacterIdleState()
{
}

IRyuCharacterState* URyuCharacterIdleState::InputPressDown(ARyuBaseCharacter* Character)
{
    if (Character->CheckOverlapClimbableActors())
    {
        return NewObject<URyuCharacterClimbState>();
    }
}

IRyuCharacterState* URyuCharacterIdleState::InputPressUp(ARyuBaseCharacter* Character)
{
    if (Character->CheckOverlapClimbableActors())
    {
        return NewObject<URyuCharacterClimbState>();
    }
}

IRyuCharacterState* URyuCharacterIdleState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
    //     IRyuCharacterState* state = URyuCharacterOnGroundState::HandleInput(Character, Input);
    //     return state;
    switch (Input)
    {
        case ERyuInputState::PressUp: {
            return InputPressUp(Character);
            break;
        }
        case ERyuInputState::PressDown: {
            return InputPressDown(Character);
            break;
        }
        default:
            return Super::HandleInput(Character, Input);
            break;
    }
}

void URyuCharacterIdleState::Update(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("Idle-State Updating."));
}

void URyuCharacterIdleState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::Idle;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterIdleState::Exit(ARyuBaseCharacter* Character)
{
}
