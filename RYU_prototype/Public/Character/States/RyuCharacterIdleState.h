// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "RyuCharacterIdleState.generated.h"

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterIdleState : public UInterface
{
	GENERATED_BODY()
public:

	URyuCharacterIdleState();
	
	//
	virtual void HandleInput(ARyuBaseCharacter& Character, const EInputEvent Input);
	virtual void Update(ARyuBaseCharacter& Character);

private:

	// TODO here we can include Stuff which is only related to this state (e.g. charging Times)



};
