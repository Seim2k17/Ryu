#pragma once

#include "CoreMinimal.h"
#include "ERyuCharacterPossibility.generated.h"

UENUM(BlueprintType)
enum class ERyuCharacterPossibility : uint8
{
	None,
	CanClimbLadderUp,
	CanClimbLadderDown,
	CanClimbLadderUpDown,
	CanClimbLedgeUp,
	CanClimbLedgeDown,
	CanClimbLedgeUpDown,
	CanInteract,
};