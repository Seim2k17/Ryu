// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "RyuCharacterState.h"
#include "RyuCharacterInAirState.generated.h"

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterInAirState : public URyuCharacterState
{
    GENERATED_BODY()
public:
    URyuCharacterInAirState();

    virtual URyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;
    virtual void Enter(ARyuBaseCharacter* Character) override{};
    virtual void Exit(ARyuBaseCharacter* Character) override;

    URyuCharacterState* InputAnimationEnded(ARyuBaseCharacter* Character) override;

    //UFUNCTION(BlueprintNativeEvent)
    ERyuCharacterState GetState() override
    // ERyuCharacterState GetState_Implementation()
    {
        return CharacterState;
    }
    // Used to get InputState not Just in the HandleInputMethod ! -> but we need to take care that it´s correctlý set !!!
    //UFUNCTION(BlueprintNativeEvent)
    void SetInputPressedState(ERyuInputState Input) override
    {
        InputPressed = Input;
    }
    //UFUNCTION(BlueprintNativeEvent)
    ERyuInputState GetInputPressedState() override
    {
        return InputPressed;
    }

private:
    // TODO here we can include Stuff which is only related to this state (e.g. charging Times)

};
