// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "IO/RyuBaseCommand.h"
#include "CoreMinimal.h"
#include "RyuJumpCommand.generated.h"

/**
 * 
 */
UCLASS()
class RYU_PROTOTYPE_API URyuJumpCommand : public URyuBaseCommand
{
    GENERATED_BODY()

    void Execute() override;
};
