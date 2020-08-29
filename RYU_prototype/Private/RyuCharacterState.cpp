// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuLookDirection.h"

// I guess we need the CPP-File to generated the "ClassName.generated.h" file !

void URyuCharacterState::FlipCharacter(ARyuBaseCharacter* Character)
{
	UE_LOG(LogRyu, Log, TEXT("FlipCharacter is called"));
	ERyuLookDirection LookDirection = Character->GetLookDirection();
	switch (InputPressed)
	{
	case ERyuInputState::PressRight:
	{
		if (LookDirection == ERyuLookDirection::Left)
		{
			Character->FlipCharacter();
		}
		UE_LOG(LogRyu, Log, TEXT("Character is walking Right."));
		break;
	}
	case ERyuInputState::PressLeft:
	{
		if (LookDirection == ERyuLookDirection::Right)
		{
			Character->FlipCharacter();
		}
		UE_LOG(LogRyu, Log, TEXT("Character is walking Left."));
		break;
	}
	default:
// 		UE_LOG(LogRyu, Log,
// 			TEXT("Hm. InputState is neither Left or Right. Its: %s at speed: %s"),
// 			*URyuStaticFunctionLibrary::InputStateToString(InputPressed),
// 			*Character->GetCharacterMovement()->Velocity.ToString());
		break;
	}
}
