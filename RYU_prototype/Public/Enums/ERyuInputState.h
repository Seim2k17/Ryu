// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

UENUM(BlueprintType)

enum class ERyuInputState : uint8
{
    PressUp,
    ReleaseUp,
    PressRight,
    ReleaseRight,
    PressDown,
    ReleaseDown,
    PressLeft,
    ReleaseLeft,
    PressJump,
    ReleaseJump,
    PressJumpUp,
    ReleaseJumpUp,
    PressJumpForward,
    ReleaseJumpForward,
    PressJumpBackward,
    ReleaseBackward,
    PressInteract,
    PressSneak,
    ReleaseSneak,
    PressAbility,
    ReleaseAbility,
    PressAttack,
    ReleasAttack,

};
