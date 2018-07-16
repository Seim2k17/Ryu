#pragma once

UENUM(BlueprintType)

enum class ERYUClimbingMode : uint8
{
	NONE UMETA(DisplayName = "none"),
	CANCLIMBUPLEDGE UMETA(DisplayName = "Can Climb Up Ledges"),
	CANCLIMBDOWNLEDGE UMETA(DisplayName = "Can Climb Down Ledges"),
	CANCLIMBUPANDDOWN UMETA(DisplayName = "Can Climb Up&&Down Ledges"),
	CLIMBDOWNLEDGE UMETA(DisplayName = "Climbing Down Ledges"),
	CLIMBUPLEDGE UMETA(DisplayName = "Climbing Up Ledges"),
	FALLDOWNLEDGE UMETA(DisplayName = "Fall Down from Ledges"),
	HANGONLEDGE UMETA(DisplayName = "Hanging On Ledges"),
	LETGOLEDGE UMETA(DisplayName = "Let go Ledges"),
	CANENTERLADDER UMETA(DisplayName = "Can enter Ladder"),
	CLIMBLADDERUP UMETA(DisplayName = "Climbing Ladder up"),
	CLIMBLADDERDOWN UMETA(DisplayName = "Climbing Ladder down"),
};