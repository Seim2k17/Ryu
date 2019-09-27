// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "RyuCharacterState.h"
#include "RyuCharacterOnGroundState.generated.h"

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterOnGroundState
    : public UObject
    , public IRyuCharacterState
{
    GENERATED_BODY()
public:
    URyuCharacterOnGroundState();

    // virtual void HandleInput(ARyuBaseCharacter& Character, const EInputEvent Input) override;

    virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;

    virtual void Enter(ARyuBaseCharacter* Character) override{};
    virtual void Exit(ARyuBaseCharacter* Character) override
    {
        InputPressed = ERyuInputState::None;
    };

    IRyuCharacterState* InputAnimationEnded(ARyuBaseCharacter* Character);

protected:
    void FlipCharacter(ARyuBaseCharacter* Character);

private:
    // TODO here we can include Stuff which is only related to this state (e.g. charging Times)
};
