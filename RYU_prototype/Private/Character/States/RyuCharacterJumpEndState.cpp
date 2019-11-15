// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterJumpEndState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterJumpEndState.h"

URyuCharacterJumpEndState::URyuCharacterJumpEndState()
{
}

URyuCharacterState* URyuCharacterJumpEndState::HandleInput(ARyuBaseCharacter* Character,
                                                           const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::AnimationEnded:
        {
            return NewObject<URyuCharacterIdleState>();
        }
    }
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
    return Super::HandleInput(Character, Input);
}

void URyuCharacterJumpEndState::Update(ARyuBaseCharacter* Character)
{
    Super::Update(Character);
}

void URyuCharacterJumpEndState::Enter(ARyuBaseCharacter* Character)
{
    Super::Enter(Character);
    CharacterState = ERyuCharacterState::JumpEnd;
    // Set JumpGraphics/Sound or other Asset related stuff / fire a event for the engine / although this is controlled by the UnrealPaperZDStateMachine (2DAssetChanges)
}

void URyuCharacterJumpEndState::Exit(ARyuBaseCharacter* Character)
{
}
