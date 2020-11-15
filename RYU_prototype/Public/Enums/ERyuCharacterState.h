// Copyright 2019 80k Games, All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "ERyuCharacterState.generated.h"


UENUM(BlueprintType)

enum class ERyuCharacterState : uint8
{
    Idle,
    Run,
	Walk,
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
	FallingNormal,
	FallingDeep,
	FallingEnd,
	FallingNormalEnd,
	FallingDeepEnd,
	FallingDeath,
    ClimbUpLadder,
    ClimbDownLadder,
    EnterLadder,
    ExitLadder,
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
