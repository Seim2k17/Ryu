// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterClimbObstacleState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"

URyuCharacterClimbObstacleState::URyuCharacterClimbObstacleState()
{
}

IRyuCharacterState* URyuCharacterClimbObstacleState::HandleInput(ARyuBaseCharacter* Character,
	const ERyuInputState Input)
{
	// 	switch (Input)
	// 	{
	//
	// 	}
	return Super::HandleInput(Character, Input);
}

void URyuCharacterClimbObstacleState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterClimbObstacleState::Enter(ARyuBaseCharacter* Character)
{
	CharacterState = ERyuCharacterState::ClimbObstacle;
	// Set IdleGraphics or other Asset related stuff
}

void URyuCharacterClimbObstacleState::Exit(ARyuBaseCharacter* Character)
{
}
