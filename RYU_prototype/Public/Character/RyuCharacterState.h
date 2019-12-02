// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "UObject/Interface.h"
#include "RyuCharacterState.generated.h"

/*
For each state, we define a class that implements the interface. Its methods define the Character’s behavior when in that state.
If otther data is related to only the specific state move it as a member in that Derived class. (e.g. ChargingTime for Firing)
*/

//UINTERFACE(BlueprintType)
UCLASS()

// class RYU_PROTOTYPE_API URyuCharacterState : public UInterface
// {
//     GENERATED_BODY()
// };
//
// class RYU_PROTOTYPE_API IRyuCharacterState

class RYU_PROTOTYPE_API URyuCharacterState : public UObject
{
    GENERATED_BODY()

public:
    // we can use References (&) in pure c++ classes but only pointers (*) in functions marked as BlueprintCallable
    // virtual void HandleInput(ARyuBaseCharacter& Character, const EInputEvent Input){};
    // UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
    // mark as pure (=0) to MUST implement them in th Class which implements the Interface
    //virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
    virtual URyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input)
    {
        return nullptr;

    }; // = 0;

    virtual void Update(ARyuBaseCharacter* Character)
    {
    } //= 0;

    virtual void Enter(ARyuBaseCharacter* Character)
    {
    } // = 0;

    virtual void Exit(ARyuBaseCharacter* Character)
    {
    } // = 0;

    //UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterState")
    //IRyuCharacterState* GetState();
    //UFUNCTION(BlueprintNati)
    //UFUNCTION(BlueprintNativeEvent)
    virtual ERyuCharacterState GetState()
    {
        ERyuCharacterState state = ERyuCharacterState::None;
        return state;
    }

    // Used to get InputState not Just in the HandleInputMethod ! -> but we need to take care that it´s correctlý set !!!
    //UFUNCTION(BlueprintNativeEvent)
    virtual void SetInputPressedState(ERyuInputState Input)
    {
    }

    //UFUNCTION(BlueprintNativeEvent)
    virtual ERyuInputState GetInputPressedState()
    {
        ERyuInputState state = ERyuInputState::None;
        return state;
    }

    /* FOR DEBUG/DEV-PURPOSE ONLY*/
    virtual void SetCharacterState(ERyuCharacterState ToCharacterState)
    {
        CharacterState = ToCharacterState;
    }

    //UFUNCTION(BlueprintNativeEvent)
    //virtual IRyuCharacterState* InputAnimationEnded(ARyuBaseCharacter* Character) = 0;
    virtual URyuCharacterState* InputAnimationEnded(ARyuBaseCharacter* Character)
    {
        return nullptr;
    }

protected:
    UPROPERTY()
    ERyuCharacterState CharacterState = ERyuCharacterState::Idle;

    UPROPERTY()
    ERyuInputState InputPressed = ERyuInputState::None;
};
