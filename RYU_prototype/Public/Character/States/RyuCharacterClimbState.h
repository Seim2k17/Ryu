// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterClimbState.generated.h"

UENUM()
enum class EClimboutState : uint8
{
    None,
    Top,
    Bottom,
    JumpOff,
};

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterClimbState
    //: public UObject
    //, public IRyuCharacterState
    : public URyuCharacterState
{
    GENERATED_BODY()
public:
    URyuCharacterClimbState();

    //virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
    virtual URyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;
    virtual void Enter(ARyuBaseCharacter* Character) override{};
    virtual void Exit(ARyuBaseCharacter* Character) override;

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

protected:
    FVector ClimbOutTop;
    FVector ClimbOutBtm;

    EClimboutState ClimbOutState;

private:
    // TODO here we can include Stuff which is only related to this state (e.g. charging Times)
};
