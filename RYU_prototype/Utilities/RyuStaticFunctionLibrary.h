// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "RyuStaticFunctionLibrary.generated.h"

class URyuMainCharacter;

UCLASS()
class URyuStaticFunctionLibrary : public UObject
{
    GENERATED_UCLASS_BODY()

    //FORCEINLINE function

    static URyuMainCharacter* GetMainChar();
};
