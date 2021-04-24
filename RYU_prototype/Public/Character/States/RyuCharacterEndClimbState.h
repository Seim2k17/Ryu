// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/States/RyuCharacterClimbState.h"
#include "CoreMinimal.h"
#include "RyuCharacterEndClimbState.generated.h"

/**
 * 
 */
UCLASS()
class RYU_PROTOTYPE_API URyuCharacterEndClimbState : public URyuCharacterClimbState
{
    GENERATED_BODY()

public:
    URyuCharacterEndClimbState();

    //virtual IRyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
    virtual URyuCharacterState* HandleInput(ARyuBaseCharacter* Character,
                                            const ERyuInputState Input) override;
    virtual void Update(ARyuBaseCharacter* Character) override;
    virtual void Enter(ARyuBaseCharacter* Character) override;
    virtual void Exit(ARyuBaseCharacter* Character) override;
};
