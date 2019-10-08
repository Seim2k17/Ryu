// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterInteractState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"

URyuCharacterInteractState::URyuCharacterInteractState()
{
}

IRyuCharacterState* URyuCharacterInteractState::HandleInput(ARyuBaseCharacter* Character,
                                                            const ERyuInputState Input)
{
    // Stuff for every Interaction
    return Super::HandleInput(Character, Input);
}

void URyuCharacterInteractState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterInteractState::Enter(ARyuBaseCharacter* Character)
{
    // Base Class of IA Things CharacterState = ERyuCharacterState::Duck;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterInteractState::Exit(ARyuBaseCharacter* Character)
{
}
