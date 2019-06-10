// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuCharacterState.h"
#include "UObject/Interface.h"
#include "RyuCharacterState.generated.h"

/*
For each state, we define a class that implements the interface. Its methods define the Character’s behavior when in that state.
If otther data is related to only the specific state move it as a member in that Derived class. (e.g. ChargingTime for Firing)
*/

UINTERFACE(BlueprintType)
class RYU_PROTOTYPE_API URyuCharacterState : public UInterface
{
    GENERATED_BODY()
};

class RYU_PROTOTYPE_API IRyuCharacterState
{
    GENERATED_BODY()

public:
    // we can use References (&) in pure c++ classes but only pointers (*) in functions marked as BlueprintCallable
    // virtual void HandleInput(ARyuBaseCharacter& Character, const EInputEvent Input){};
    // UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
    // mark as pure (=0) to MUST implement them in th Class which implements the Interface
    virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) = 0;

    virtual void Update(ARyuBaseCharacter* Character) = 0;

    virtual void Enter(ARyuBaseCharacter* Character) = 0;

    virtual void Exit(ARyuBaseCharacter* Character) = 0;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterState")
    ERyuCharacterState GetState();
	virtual ERyuCharacterState GetState_Implementation() = 0;
	//	bool GetState();

protected:

	ERyuCharacterState CharacterState = ERyuCharacterState::Idle;
};
