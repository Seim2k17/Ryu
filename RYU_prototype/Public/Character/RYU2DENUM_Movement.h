#pragma once


UENUM(BlueprintType)

enum class EPlayerMovement : uint8
{
	STAND UMETA(DisplayName = "Standing"),
	WALK UMETA(DisplayName = "Walking"),
	STARTTURN UMETA(DisplayName = "Start Turning"),
	TURN UMETA(DisplayName = "Turning"),
	BEGINRUN UMETA(DisplayName = "Begin Running"),
	RUN UMETA(DisplayName = "Running Loop"),
	ENDRUN UMETA(DisplayName = "End Running"),
	JUMPUP UMETA(DisplayName = "Jump Up"),
	FALLING UMETA(DisplayName = "Fallng"),
	HANGINGLEDGE UMETA(DisplayName = "Hangon on Ledge"),
	CLIMBING UMETA(DisplayName = "Climbing"),
	CANGRABLEDGE UMETA(DisplayName = "Can grab ledge"),
	SNEAK UMETA(DisplayName = "Sneak"),
};