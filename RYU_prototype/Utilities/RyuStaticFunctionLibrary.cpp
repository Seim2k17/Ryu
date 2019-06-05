// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuStaticFunctionLibrary.h"
#include "RyuMainCharacter.h"
#include <Components/ActorComponent.h>

URyuStaticFunctionLibrary::URyuStaticFunctionLibrary(const FObjectInitializer& Objectinitializer)
    : Super(Objectinitializer)
{
}

ARyuMainCharacter* URyuStaticFunctionLibrary::GetMainChar(AActor* actor)
{
    return Cast<ARyuMainCharacter>(actor);
}

ARyuMainCharacter* URyuStaticFunctionLibrary::GetMainCharOwner(UActorComponent* Component)
{
    return Cast<ARyuMainCharacter>(Component->GetOwner());
}
