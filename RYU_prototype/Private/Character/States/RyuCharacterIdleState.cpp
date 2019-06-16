// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterIdleState.h"
#include "Character/RyuBaseCharacter.h"
#include "Components/RyuClimbingComponent.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuInteractionStatus.h"
#include "Enums/ERyuLedgePosition.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterClimbState.h"
#include "RyuCharacterCloseState.h"
#include "RyuCharacterOpenState.h"
#include "RyuCharacterPullState.h"
#include "RyuCharacterPushState.h"
#include "RyuCharacterState.h"
#include "RyuCharacterTalkState.h"

URyuCharacterIdleState::URyuCharacterIdleState()
{
}

IRyuCharacterState* URyuCharacterIdleState::InputPressDown(ARyuBaseCharacter* Character)
{
    if (auto* ClimbingComp = Character->FindComponentByClass<URyuClimbingComponent>())
    {
        if (Character->CheckOverlapClimbableActors()
            && ((ClimbingComp->GetLedgePosition() == ERyuLedgePosition::PosiDown)
                || (ClimbingComp->GetLedgePosition() == ERyuLedgePosition::PosiUpDown)))
        {
            //TODO: is this needable ??
            // RyuClimbingComponent->SetClimbingState(ERYUClimbingMode::CANCLIMBDOWNLEDGE);

            return NewObject<URyuCharacterClimbState>();
        }
    }

    return Super::HandleInput(Character, ERyuInputState::PressDown);
}

IRyuCharacterState* URyuCharacterIdleState::InputPressUp(ARyuBaseCharacter* Character)
{
    if (auto* ClimbingComp = Character->FindComponentByClass<URyuClimbingComponent>())
    {
        if (Character->CheckOverlapClimbableActors()
            && ((ClimbingComp->GetLedgePosition() == ERyuLedgePosition::PosiUp)
                || (ClimbingComp->GetLedgePosition() == ERyuLedgePosition::PosiUpDown)))
        {
            // RyuClimbingComponent->SetClimbingState(ERYUClimbingMode::CANCLIMBUPLEDGE);

            return NewObject<URyuCharacterClimbState>();
        }
    }
    return Super::HandleInput(Character, ERyuInputState::PressUp);
}

IRyuCharacterState* URyuCharacterIdleState::InputPressInteract(ARyuBaseCharacter* Character)
{
    ERyuInteractionStatus IAStatus = Character->GetInteractionStatus();
    switch (IAStatus)
    {
        case ERyuInteractionStatus::Close:
        {
            return NewObject<URyuCharacterCloseState>();
            break;
        }
        case ERyuInteractionStatus::Open:
        {
            return NewObject<URyuCharacterOpenState>();
            break;
        }
        case ERyuInteractionStatus::Pull:
        {
            return NewObject<URyuCharacterPullState>();
            break;
        }
        case ERyuInteractionStatus::Push:
        {
            return NewObject<URyuCharacterPushState>();
            break;
        }
        case ERyuInteractionStatus::Talk:
        {
            return NewObject<URyuCharacterTalkState>();
            break;
        }
        default:
        {
            return Super::HandleInput(Character, ERyuInputState::PressInteract);
            break;
        }
    }
}

IRyuCharacterState* URyuCharacterIdleState::InputPressAbility(ARyuBaseCharacter* Character)
{
    //TODO: which Ability is selected
	return nullptr;
}

IRyuCharacterState* URyuCharacterIdleState::InputPressAttack(ARyuBaseCharacter* Character)
{
    if (Character->IsInCombat())
    {
        //TODO which Attacks can be performed, atm no StateChanges
        return nullptr; //IRyuCharacterCombatState
    }

    return nullptr;
}

IRyuCharacterState* URyuCharacterIdleState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
    //     IRyuCharacterState* state = URyuCharacterOnGroundState::HandleInput(Character, Input);
    //     return state;
    switch (Input)
    {
        case ERyuInputState::PressUp:
        {
            return InputPressUp(Character);
            break;
        }
        case ERyuInputState::PressDown:
        {
            return InputPressDown(Character);
            break;
        }
        case ERyuInputState::PressInteract:
        {
            return InputPressInteract(Character);
        }
        case ERyuInputState::PressAbility:
        {
            return InputPressAbility(Character);
        }
        case ERyuInputState::PressAttack:
        {
            return InputPressAttack(Character);
        }
        default:
            return Super::HandleInput(Character, Input);
            break;
    }
}

void URyuCharacterIdleState::Update(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("Idle-State Updating."));
}

void URyuCharacterIdleState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::Idle;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterIdleState::Exit(ARyuBaseCharacter* Character)
{
}
