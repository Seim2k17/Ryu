#pragma once

UENUM(BlueprintType)

enum class ERYUMovementMode : uint8
{
	STAND UMETA(DisplayName = "Standing"),
	WALK	UMETA(DisplayName = "Walking"),
	RUN UMETA(DisplayName = "Running"),
	JUMP UMETA(DisplayName = "Jumping"),
	CLIMBLADDER UMETA(DisplayName = "Climbing Ladder"),
	CROUCH UMETA(DisplayName = "Crouching"),
	PUSHPULL UMETA(DisplayName = "Push Pull"),
	CANTRACELEDGE UMETA(DisplayName = "Trace Ledges"),
	CANGRABLEDGE UMETA(DisplayName = "Grab Ledges"),
	HANGONLEDGE UMETA(DisplayName = "Hanging On Ledges"),
	CANCLIMBDOWNLEDGE UMETA(DisplayName = "Can Climb Down Ledges"),
	CANCLIMBUPLEDGE UMETA(DisplayName = "Can Climb Up Ledges"),
	CLIMBDOWNLEDGE UMETA(DisplayName = "Climbing Down Ledges"),
	CLIMBUPLEDGE UMETA(DisplayName = "Climbing Up Ledges"),
	FALLDOWNLEDGE UMETA(DisplayName = "Fall Down from Ledges"),
};