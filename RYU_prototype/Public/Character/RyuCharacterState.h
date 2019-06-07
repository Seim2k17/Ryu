// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Character/RyuBaseCharacter.h"
#include <Engine/EngineBaseTypes.h>
#include "RyuCharacterState.generated.h"

UINTERAFCE(BlueprintType)
class RYU_PROTOTYPE_API URyuCharacterState : public UInterface
{
    GENERATED_BODY()
public:
    virtual ~URyuCharacterState(){};
    virtual void HandleInput(ARyuBaseCharacter& Character, const EInputEvent Input){};
    virtual void Update(ARyuBaseCharacter& Character){};
};
