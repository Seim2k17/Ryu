// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMovementState.h"
#include "RyuCharacterJumpState.h"
#include "RyuCharacterJumpForwardState.generated.h"

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterJumpForwardState : public URyuCharacterJumpState
{
    GENERATED_BODY()
public:
    URyuCharacterJumpForwardState();

    URyuCharacterJumpForwardState(ERyuMovementState MovementState);

    // virtual void HandleInput(ARyuBaseCharacter& Character, const EInputEvent Input) override;

    virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;
    virtual void Enter(ARyuBaseCharacter* Character) override;
    virtual void Exit(ARyuBaseCharacter* Character) override;

    void SetCharacterMovement(ERyuMovementState JumpMovementState);

private:
    // TODO here we can include Stuff which is only related to this state (e.g. charging Times)
    ERyuMovementState CharacterMovementState = ERyuMovementState::Standing;
};
