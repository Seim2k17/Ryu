// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "RyuCharacterInteractState.h"
#include "RyuCharacterOpenState.generated.h"

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterOpenState : public URyuCharacterInteractState
{
	GENERATED_BODY()
public:
	URyuCharacterOpenState();

	// virtual void HandleInput(ARyuBaseCharacter& Character, const EInputEvent Input) override;

	virtual URyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
		const ERyuInputState Input) override;
	virtual void Update(ARyuBaseCharacter* Character) override;
	virtual void Enter(ARyuBaseCharacter* Character) override;
	virtual void Exit(ARyuBaseCharacter* Character) override;

private:
	// TODO here we can include Stuff which is only related to this state (e.g. charging Times)
};

