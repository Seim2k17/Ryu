// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterClimbObstcaleState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"

URyuCharacterClimbObstcaleState::URyuCharacterClimbObstcaleState()
{
}

IRyuCharacterState* URyuCharacterClimbObstcaleState::HandleInput(ARyuBaseCharacter* Character,
	const ERyuInputState Input)
{
	// 	switch (Input)
	// 	{
	//
	// 	}
	return Super::HandleInput(Character, Input);
}

void URyuCharacterClimbObstcaleState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterClimbObstcaleState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::ClimbObstacle;
	// Set IdleGraphics or other Asset related stuff
}

void URyuCharacterClimbObstcaleState::Exit(ARyuBaseCharacter* Character)
{
}
