// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

UENUM(BlueprintType)

enum class ERyuCharacterState : uint8
{
    Idle,
    Run,
    Sprint,
    Sneak,
    Duck,
	DuckMove,
	Roll,
    JumpForward,
    JumpForwardFast,
    JumpBackward,
    JumpBackwardFast,
    JumpUpward,
	JumpEnd,
    ClimbLedge,
    ClimbObstacle,
	IdleMoveObject,
    Push,
    Pull,
    Combat,
    Ability,
    Open,
    Close,
    Talk,
    Death,
	None
};
