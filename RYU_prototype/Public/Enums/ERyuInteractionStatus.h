// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

UENUM(BlueprintType)

enum class ERyuInteractionStatus : uint8
{
	None,
    Talk,
    Open,
    Close,
    Push,
    Pull,

};
