#pragma once

UENUM(BlueprintType)

enum class ERYULedgePosition : uint8
{
	NONE UMETA(DisplayName = "none"),
	PosiUp UMETA(DisplayName = "UpLedge"),
	PosiDown UMETA(DisplayName = "DownLedge"),
	PosiUpDown UMETA(DisplayName = "UpAndDownLedge"),
};
