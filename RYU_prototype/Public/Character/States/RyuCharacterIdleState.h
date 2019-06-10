// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include "Enums/ERyuInputState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuCharacterIdleState.generated.h"

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

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterState")
    URyuCharacterOnGroundState* GetState();

    URyuCharacterOnGroundState* GetState_Implementation() override{return this};

private:
    // TODO here we can include Stuff which is only related to this state (e.g. charging Times)
};
