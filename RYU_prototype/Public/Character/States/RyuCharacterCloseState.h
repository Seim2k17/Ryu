// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterCloseState.generated.h"

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterCloseState : public URyuCharacterOnGroundState
{
	GENERATED_BODY()
public:
	URyuCharacterCloseState();

	//virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
	virtual URyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
		const ERyuInputState Input) override;
	virtual void Update(ARyuBaseCharacter* Character) override;
	virtual void Enter(ARyuBaseCharacter* Character) override;
	virtual void Exit(ARyuBaseCharacter* Character) override;

private:
	// TODO here we can include Stuff which is only related to this state (e.g. charging Times)
};
