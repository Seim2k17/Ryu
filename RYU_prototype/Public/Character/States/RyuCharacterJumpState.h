// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterJumpState.generated.h"

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterJumpState
    : public UObject
    , public IRyuCharacterState
{
    GENERATED_BODY()
public:
    URyuCharacterJumpState();

    virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;
    virtual void Enter(ARyuBaseCharacter* Character) override;
    virtual void Exit(ARyuBaseCharacter* Character) override{};

    IRyuCharacterState* InputAnimationEnded(ARyuBaseCharacter* Character);

private:
    // TODO here we can include Stuff which is only related to this state (e.g. charging Times)
};
