// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/States/RyuCharacterInAirState.h"
#include "CoreMinimal.h"
#include "RyuCharacterFallingEndState.generated.h"

/**
 * 
 */
UCLASS()
class RYU_PROTOTYPE_API URyuCharacterFallingEndState : public URyuCharacterInAirState
{
    GENERATED_BODY()

public:
    URyuCharacterFallingEndState();

    virtual URyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;
    virtual void Enter(ARyuBaseCharacter* Character) override;
    virtual void Exit(ARyuBaseCharacter* Character) override;

private:
};
