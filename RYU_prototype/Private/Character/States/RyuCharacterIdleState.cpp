// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterIdleState.h"
#include "Character/RyuBaseCharacter.h"
#include "Components/RyuClimbingComponent.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuInteractionStatus.h"
#include "Enums/ERyuLedgePosition.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterAbilityState.h"
#include "RyuCharacterClimbState.h"
#include "RyuCharacterCloseState.h"
#include "RyuCharacterDuckState.h"
#include "RyuCharacterIdleMoveObjectState.h"
#include "RyuCharacterJumpBackwardState.h"
#include "RyuCharacterJumpForwardState.h"
#include "RyuCharacterJumpUpwardState.h"
#include "RyuCharacterOpenState.h"
#include "RyuCharacterRunState.h"
#include "RyuCharacterSneakState.h"
#include "RyuCharacterSprintState.h"
#include "RyuCharacterTalkState.h"

URyuCharacterIdleState::URyuCharacterIdleState()
{
}

void URyuCharacterIdleState::Enter(ARyuBaseCharacter* Character)
{
    CharacterState = ERyuCharacterState::Idle;
    // Set IdleGraphics or other Asset related stuff
}

void URyuCharacterIdleState::Exit(ARyuBaseCharacter* Character)
{
}

IRyuCharacterState* URyuCharacterIdleState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
    //     IRyuCharacterState* state = URyuCharacterOnGroundState::HandleInput(Character, Input);
    //     return state;
    switch (Input)
    {
        case ERyuInputState::PressLeft:
        case ERyuInputState::PressRight:
        {
            return InputPressLeftRight(Character, Input);
            break;
        }
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
        case ERyuInputState::PressJump:
        case ERyuInputState::PressJumpBackward:
        case ERyuInputState::PressJumpUp:
        case ERyuInputState::PressJumpForward:
        {
            return InputPressJump(Character);
        }
        default:
            return Super::HandleInput(Character, Input);
            break;
    }
}

IRyuCharacterState* URyuCharacterIdleState::InputPressAbility(ARyuBaseCharacter* Character)
{
    //TODO: which Ability is selected
    return NewObject<URyuCharacterAbilityState>();
}

IRyuCharacterState* URyuCharacterIdleState::InputPressAttack(ARyuBaseCharacter* Character)
{
    if (Character->EnemyInSight())
    {
        return NewObject<URyuCharacterCombatState>();
    }
    return nullptr;
}

IRyuCharacterState* URyuCharacterIdleState::InputPressLeftRight(ARyuBaseCharacter* Character,
                                                                const ERyuInputState Input)
{
    ERyuMovementState CharacterMovement = Character->GetCharacterMovementState();
    switch (CharacterMovement)
    {
        case ERyuMovementState::Sneaking:
        {
            return NewObject<URyuCharacterSneakState>();
            break;
        }
        case ERyuMovementState::Sprinting:
        {
            return NewObject<URyuCharacterSprintState>();
            break;
        }
        default:
            break;
    }
    return NewObject<URyuCharacterRunState>();
}

IRyuCharacterState* URyuCharacterIdleState::InputPressDown(ARyuBaseCharacter* Character)
{
    // if Climbing is possible we want to climb down
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

    // we later decide what we move to the BaseClass
    // return Super::HandleInput(Character, ERyuInputState::PressDown);
    return NewObject<URyuCharacterDuckState>();
}

IRyuCharacterState* URyuCharacterIdleState::InputPressInteract(ARyuBaseCharacter* Character)
{
    ERyuInteractionStatus InteractStatus = Character->GetInteractionStatus();
    switch (InteractStatus)
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
        case ERyuInteractionStatus::MoveObject:
        {
            return NewObject<URyuCharacterIdleMoveObjectState>();
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

IRyuCharacterState* URyuCharacterIdleState::InputPressJump(ARyuBaseCharacter* Character)
{
    switch (InputPressed)
    {
        case ERyuInputState::PressJump:
        case ERyuInputState::PressJumpUp:
            return NewObject<URyuCharacterJumpUpwardState>();
        case ERyuInputState::PressJumpBackward:
            return NewObject<URyuCharacterJumpBackwardState>(false);
        case ERyuInputState::PressJumpForward:
            return NewObject<URyuCharacterJumpForwardState>(false);
        default:
            return nullptr;
            break;
    }
}

IRyuCharacterState* URyuCharacterIdleState::InputPressUp(ARyuBaseCharacter* Character)
{
    // if Climbing is possible we want to climb up
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

void URyuCharacterIdleState::Update(ARyuBaseCharacter* Character)
{
    // LOG(LogRyu, Log, TEXT("Idle-State Updating."));
}
