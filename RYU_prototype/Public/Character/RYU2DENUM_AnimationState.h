#pragma once

UENUM(BlueprintType)

enum class ERYU2DAnimationState: uint8
{
	IDLE UMETA(DisplayName = "Idle"),
	STARTTURN UMETA(DisplayName = "StartTurning"),
	BEGINRUN UMETA(DisplayName = "BeginRun"),
	RUNNING UMETA(DisplayName = "LoopRun"),
	ENDRUN UMETA(DisplayName = "EndRun"),
	CLIMBING UMETA(DisplayName = "Climbing"),
	JUMPUP UMETA(DisplayName = "JumpUp"),
	JUMPFORWARD UMETA(DisplayName = "JumpForward"),
	FALLINGIN UMETA(DisplayName = "Falling In"),
	FALLINGOUT UMETA(DisplayName = "Falling Out"),
	FALLINGLOOP UMETA(DisplayName = "Falling Loop"),
};
