// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/States/RyuCharacterClimbState.h"
#include "Enums/ERyuInputState.h"
#include "CoreMinimal.h"
#include "RyuCharacterClimbLadderState.generated.h"

class URyuCharacterState;
class ARyuBaseCharacter;

/**
 * 
 */

UCLASS()
class RYU_PROTOTYPE_API URyuCharacterClimbLadderState : public URyuCharacterClimbState
{
    GENERATED_BODY()

    URyuCharacterClimbLadderState();

    //virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
    virtual URyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;
    virtual void Enter(ARyuBaseCharacter* Character) override;
    virtual void Exit(ARyuBaseCharacter* Character) override;

private:
    // TODO here we can include Stuff which is only related to this state (e.g. charging Times)
 
};
