// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "RyuStaticFunctionLibrary.generated.h"

class ARyuMainCharacter;
class UActorComponent;

UCLASS()
class URyuStaticFunctionLibrary : public UObject
{
    GENERATED_UCLASS_BODY()
public:
    //FORCEINLINE function

    static ARyuMainCharacter* GetMainChar(AActor* actor);

    static ARyuMainCharacter* GetMainCharOwner(UActorComponent* Component);
};
