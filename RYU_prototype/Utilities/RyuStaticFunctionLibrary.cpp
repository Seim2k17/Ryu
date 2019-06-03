// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuStaticFunctionLibrary.h"
#include "RyuMainCharacter.h"

URyuMainCharacter* URyuStaticFunctionLibrary::GetMainChar(AActor* actor)
{
	return Cast<URyuMainCharacter>(actor);
}
