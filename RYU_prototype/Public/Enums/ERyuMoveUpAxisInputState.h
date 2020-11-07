// Copyright 2019 80k Games, All Rights Reserved.


#include "CoreMinimal.h"
#include "ERyuMoveUpAxisInputState.generated.h"

#pragma once

UENUM(BlueprintType)
enum class ERyuMoveUpAxisInputState : uint8
{
	Inactive,
	PressUpAxisKey,
	ReleaseUpAxisKey,
	PressDownAxisKey,
	ReleaseDownAxisKey,
};