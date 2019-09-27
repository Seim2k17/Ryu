// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterAttackState.h"
#include "Enums/ERyuInputState.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"

URyuCharacterAttackState::URyuCharacterAttackState()
{
}

IRyuCharacterState* URyuCharacterAttackState::HandleInput(ARyuBaseCharacter* Character,
                                                          const ERyuInputState Input)
{
    // TODO: Check which Attack and return appr. AttackState
    switch (Input)
    {
        case ERyuInputState::AnimationEnded:
        {
            return InputAnimationEnded(Character); //InputAnimationEnde(Character);
        }
        default:
        {
            return nullptr;
            break;
        }
    }
}

void URyuCharacterAttackState::Update(ARyuBaseCharacter* Character)
{
}

IRyuCharacterState* URyuCharacterAttackState::InputAnimationEnded(ARyuBaseCharacter* Character)
{
	// TODO add app AnimationEndedKram
    return nullptr;
}
