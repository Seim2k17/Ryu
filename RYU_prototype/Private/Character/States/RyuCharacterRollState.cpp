// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterRollState.h"
#include "Enums/ERyuInputState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterOnGroundState.h"
#include "RyuMainCharacter.h"

URyuCharacterRollState::URyuCharacterRollState()
{
}

IRyuCharacterState* URyuCharacterRollState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
    switch (Input)
    {
        // TODO TEST IF THIS REALLY WORKS AS A CALLBACK IF WE CAME FROM RUNNING AND RELEASE MOVEMENTDIRECTION->IDLE
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseRight:
        {
            return NewObject<URyuCharacterIdleState>();
            break;
        }
        case ERyuInputState::PressJump:
        {
            return NewObject<URyuCharacterJumpForwardState>(false);
            break;
        }
    }

    return nullptr;
}

void URyuCharacterRollState::Update(ARyuBaseCharacter* Character)
{
    if (auto* MainChar = Cast<ARyuMainCharacter>(Character))
    {
        float MoveRightInput = MainChar->GetMoveRightInput();
        MainChar->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveRightInput);
    }
}

void URyuCharacterRollState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::Roll;
}

void URyuCharacterRollState::Exit(ARyuBaseCharacter* Character)
{
    Super::Exit(Character);
}
