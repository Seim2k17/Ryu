#pragma once

UENUM(BlueprintType)

enum class ERYUEMovementMode : uint8
{
	Stand UMETA(DisplayName = "Standing"),
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running"),
	Jumping UMETA(DisplayName = "Jumping"),
	OnLadder UMETA(DisplayName = "ClimbingLadder"),
	Crouching UMETA(DisplayName = "Crouching"),
	PushPull UMETA(DisplayName = "PushingPulling"),
	CanTraceLedge UMETA(DisplayName = "CanTraceLedge"),
	CanGrabLedge UMETA(DisplayName = "CanGrabLedge"),
	HangingOnLedge UMETA(DisplayName = "HangingOnLedge"),
	ClimbingLedge UMETA(DisplayName = "ClimbingLedge"),
};