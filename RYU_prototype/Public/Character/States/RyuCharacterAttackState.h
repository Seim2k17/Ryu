// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
//#include "RyuCharacterState.h"
#include "RyuCharacterAttackState.generated.h"

class URyuCharacterState;

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterAttackState
    //: public UObject
    //, public IRyuCharacterState
	: public URyuCharacterState
{
    GENERATED_BODY()
public:
    URyuCharacterAttackState();

    //virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
	virtual URyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;

    virtual void Enter(ARyuBaseCharacter* Character) override{};
    virtual void Exit(ARyuBaseCharacter* Character) override{};

    //IRyuCharacterState* InputAnimationEnded(ARyuBaseCharacter* Character) override;
	URyuCharacterState* InputAnimationEnded(ARyuBaseCharacter* Character) override;

    //UFUNCTION(BlueprintNativeEvent)
    ERyuCharacterState GetState() override
    // ERyuCharacterState GetState_Implementation()
    {
        return CharacterState;
    }
    // Used to get InputState not Just in the HandleInputMethod ! -> but we need to take care that it�s correctl� set !!!
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
