#pragma once

UENUM(BlueprintType)

enum class ERYUMovementMode : uint8
{
	STAND UMETA(DisplayName = "Standing"),
	WALK	UMETA(DisplayName = "Walking"),
	RUN UMETA(DisplayName = "Running"),
	JUMP UMETA(DisplayName = "Jumping"),
	CANGRABLEDGE UMETA(DisplayName = "CanGrabLedge"),
	CANTRACELEDGE UMETA(DisplayName = "CanTraceLedge"),
	CLIMB UMETA(DisplayName = "Climbing"),
	CROUCH UMETA(DisplayName = "Crouching"),
	PUSHPULL UMETA(DisplayName = "Push Pull"),
};