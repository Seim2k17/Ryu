// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterIdleState.generated.h"

class ARyuBaseCharacter;
class IRyuCharacterState;

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterIdleState : public URyuCharacterOnGroundState
{
    GENERATED_BODY()
public:
    URyuCharacterIdleState();

    // virtual void HandleInput(ARyuBaseCharacter& Character, const EInputEvent Input) override;

    virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;
    virtual void Enter(ARyuBaseCharacter* Character) override;
    virtual void Exit(ARyuBaseCharacter* Character) override;

private:
    IRyuCharacterState* InputPressLeftRight(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input);
    IRyuCharacterState* InputPressDown(ARyuBaseCharacter* Character);
    IRyuCharacterState* InputPressUp(ARyuBaseCharacter* Character);
    IRyuCharacterState* InputPressInteract(ARyuBaseCharacter* Character);
    IRyuCharacterState* InputPressAbility(ARyuBaseCharacter* Character);
    IRyuCharacterState* InputPressAttack(ARyuBaseCharacter* Character);
    IRyuCharacterState* InputPressJump(ARyuBaseCharacter* Character);
    // TODO here we can include Stuff which is only related to this state (e.g. charging Times)
};
