// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterClimbLedgeState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"

URyuCharacterClimbLedgeState::URyuCharacterClimbLedgeState()
{
}

IRyuCharacterState* URyuCharacterClimbLedgeState::HandleInput(ARyuBaseCharacter* Character,
                                                              const ERyuInputState Input)
{
    // 	switch (Input)
    // 	{
    //
    // 	}
    return Super::HandleInput(Character, Input);
}

void URyuCharacterClimbLedgeState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterClimbLedgeState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::ClimbLedge;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterClimbLedgeState::Exit(ARyuBaseCharacter* Character)
{
}
