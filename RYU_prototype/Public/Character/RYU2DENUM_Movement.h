#pragma once


UENUM(BlueprintType)

enum class EPlayerMovement : uint8
{
	STAND UMETA(DisplayName = "Standing"),
	WALK UMETA(DisplayName = "Walking"),
	STARTTURN UMETA(DisplayName = "Start Turning"),
	ENDTURN UMETA(DisplayName = "End Turning"),
	BEGINRUN UMETA(DisplayName = "Begin Running"),
	RUN UMETA(DisplayName = "Running Loop"),
	ENDRUN UMETA(DisplayName = "End Running"),
	JUMPUP UMETA(DisplayName = "Jump Up"),
	STARTFALLING UMETA(DisplayName = "StartFalling"),
	FALLING UMETA(DisplayName = "Falling"),
	CLIMBING UMETA(DisplayName = "Climbing"),
	CANGRABLEDGE UMETA(DisplayName = "Can grab ledge"),
	SNEAK UMETA(DisplayName = "Sneak"),
	STANDUP UMETA(DisplayName = "StandUp"),
	COMBAT UMETA(DisplayName = "Combat"),
};