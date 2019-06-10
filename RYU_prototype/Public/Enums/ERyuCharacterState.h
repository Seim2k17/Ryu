// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

UENUM(BlueprintType)

enum class ERyuCharacterState : uint8
{
	Idle,
	Walking,
	Running,
	Sprinting,
	Ducking,
};
