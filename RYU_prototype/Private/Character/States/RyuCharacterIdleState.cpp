// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterIdleState.h"
#include "Character/RyuBaseCharacter.h"
#include "Components/RyuClimbingComponent.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuInteractionStatus.h"
#include "Enums/ERyuLedgePosition.h"
#include "Enums/ERyuMovementState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
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
    //TODO: is it a bit reduntant (for JumpingForwardStates....) we can use the normal state instead or not ?
    Character->SetCharacterMovementState(ERyuMovementState::Standing);
    Character->JumpToAnimInstanceNode(Character->IdleNodeName);
    // Set IdleGraphics or other Asset related stuff

    Character->GetSprite()->SetRelativeLocation(Character->IdleSpriteRelativePosition);
    Character->GetCapsuleComponent()->SetCapsuleHalfHeight(Character->IdleCapsuleHeight);
}

void URyuCharacterIdleState::Exit(ARyuBaseCharacter* Character)
{
    this->ConditionalBeginDestroy();
}

URyuCharacterState* URyuCharacterIdleState::HandleInput(ARyuBaseCharacter* Character,
                                                        const ERyuInputState Input)
{
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input,
    // just walk up in the hierarchy
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
            break;
        }
        case ERyuInputState::PressAbility:
        {
            return InputPressAbility(Character);
            break;
        }
        case ERyuInputState::PressAttack:
        {
            return InputPressAttack(Character);
            break;
        }
        case ERyuInputState::PressJump:
        case ERyuInputState::PressJumpBackward:
        case ERyuInputState::PressJumpUp:
        case ERyuInputState::PressJumpForward:
        {
            return InputPressJump(Character);
            break;
        }

        default:
        {
            return Super::HandleInput(Character, Input);
            break;
        }
    }
}

URyuCharacterState* URyuCharacterIdleState::InputPressAbility(ARyuBaseCharacter* Character)
{
    //TODO: which Ability is selected
    return NewObject<URyuCharacterAbilityState>();
}

URyuCharacterState* URyuCharacterIdleState::InputPressAttack(ARyuBaseCharacter* Character)
{
    if (Character->EnemyInSight())
    {
        return NewObject<URyuCharacterCombatState>();
    }
    return this;
}

URyuCharacterState* URyuCharacterIdleState::InputPressLeftRight(ARyuBaseCharacter* Character,
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

URyuCharacterState* URyuCharacterIdleState::InputPressDown(ARyuBaseCharacter* Character)
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

URyuCharacterState* URyuCharacterIdleState::InputPressInteract(ARyuBaseCharacter* Character)
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

URyuCharacterState* URyuCharacterIdleState::InputPressJump(ARyuBaseCharacter* Character)
{
    //     UE_LOG(LogRyu, Error, TEXT("FromCharIdleState: Inputpressed: %s"),
    //            *URyuStaticFunctionLibrary::InputStateToString(InputPressed));
    switch (InputPressed)
    { // TODO: appr. dist. btw. jumpup/fwd/fwd_fast
        case ERyuInputState::PressJump:
        case ERyuInputState::PressJumpUp:
            return NewObject<URyuCharacterJumpUpwardState>();
        case ERyuInputState::PressJumpBackward:
            return NewObject<URyuCharacterJumpBackwardState>();
        case ERyuInputState::PressJumpForward:
            return NewObject<URyuCharacterJumpForwardState>();
        default:
            return this;
            break;
    }
}

URyuCharacterState* URyuCharacterIdleState::InputPressUp(ARyuBaseCharacter* Character)
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
    Super::Update(Character);
    // LOG(LogRyu, Log, TEXT("Idle-State Updating."));
    // check if MoveRightLeft is still pressed, when entering this State:
    // DO WE NEED TO CALL THIS IN UPDATE ??? -> everytime HandleInput os called A new State is created !!!
    // -> unnecessa. StateDestroy & Creation (it´s the same State each frame!)
    if (auto MainChar = URyuStaticFunctionLibrary::GetMainChar(Character))
    {
        switch (MainChar->GetMoveRightAxisState())
        {
            case ERyuMoveRightAxisInputState::PressLeftAxisKey:
            {
                MainChar->HandleInput(ERyuInputState::PressLeft);
            }
            case ERyuMoveRightAxisInputState::PressRightAxisKey:
            {
                MainChar->HandleInput(ERyuInputState::PressRight);
            }
        }
        /*
        if (MainChar->GetMoveRightInput() < 0)
        {
            MainChar->HandleInput(ERyuInputState::PressLeft);
            return;
        }
        if (MainChar->GetMoveRightInput() > 0)
        {
            MainChar->HandleInput(ERyuInputState::PressRight);
            return;
        }
		*/
    }

    //return InputPressLeftRight(Character, Input);
}
