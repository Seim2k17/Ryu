// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterRunState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMovementState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterIdleState.h"
#include "RyuCharacterJumpForwardState.h"
#include "RyuCharacterRollState.h"
#include "RyuCharacterSneakState.h"
#include "RyuCharacterSprintState.h"
#include "RyuMainCharacter.h"

URyuCharacterRunState::URyuCharacterRunState()
{
}

URyuCharacterState* URyuCharacterRunState::HandleInput(ARyuBaseCharacter* Character,
                                                       const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::PressDown:
        {
            return NewObject<URyuCharacterRollState>();
            break;
        }
        case ERyuInputState::PressJump:
        {
            return NewObject<URyuCharacterJumpForwardState>(); // ERyuMovementState::Running
            break;
        }
        case ERyuInputState::ReleaseLeft:
        case ERyuInputState::ReleaseRight:
        {
            return NewObject<URyuCharacterIdleState>();
            break;
        }
        case ERyuInputState::PressSneakLeft:
        case ERyuInputState::PressSneakRight:
        {
            return NewObject<URyuCharacterSneakState>();
            break;
        }
        case ERyuInputState::PressSprintLeft:
        case ERyuInputState::PressSprintRight:
        {
            if (Character->GetCharacterStatus(ERyuCharacterStatus::Stamina) > 0.0f)
            {
                return NewObject<URyuCharacterSprintState>();
            }
            return Super::HandleInput(Character, Input);
            break;
        }
        default:
        {
           // UE_LOG(LogRyu, Log, TEXT("DEFAULT@RUNSTATE."));
            return Super::HandleInput(Character, Input);
            break;
        }
    }
}

void URyuCharacterRunState::Update(ARyuBaseCharacter* Character)
{
    Super::Update(Character);

    if (auto* MainChar = Cast<ARyuMainCharacter>(Character))
    {
        float MoveRightInput = MainChar->GetMoveRightInput();
        //UE_LOG(LogRyu, Log, TEXT("RunState: AddMovementInput: %f"), MoveRightInput);
		if (FMath::Abs(MoveRightInput) < 0.5f)
		{
			//walking?: Character->SetCharacterMovementState(ERyuMovementState::Sneaking);
			CharacterState = ERyuCharacterState::Walk;
		}
		else
		{
			CharacterState = ERyuCharacterState::Run;
		}
		MainChar->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveRightInput);
		
        //MainChar->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), 1.0f);

        //UE_LOG(LogRyu, Log, TEXT("Character runs with %s "), *Character->GetVelocity().ToString());
    }
}

void URyuCharacterRunState::Enter(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("CharRunState::Enter() InputState: %s"),
           *URyuStaticFunctionLibrary::InputStateToString(InputPressed));
    Super::Enter(Character);
    Super::FlipCharacter(Character);
	Character->SetJumpAllowed(true);

    if (auto* MainChar = URyuStaticFunctionLibrary::GetMainChar(Character))
    {
        if (MainChar->GetMoveRightInput() == 0.0f)
        {
            MainChar->ResetMoveRightInput();
        }
    }

    Character->SetCharacterMovementState(ERyuMovementState::Running);
    CharacterState = ERyuCharacterState::Run;

    if (Character->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::PressLeftAxisKey)
    {
        InputPressed = ERyuInputState::PressLeft;
    }
    else if (Character->GetMoveRightAxisState() == ERyuMoveRightAxisInputState::PressRightAxisKey)
    {
        InputPressed = ERyuInputState::PressRight;
    }

    Character->JumpToAnimInstanceNode(Character->RunNodeName);
}

void URyuCharacterRunState::Exit(ARyuBaseCharacter* Character)
{
    Super::Exit(Character);
    //	this->ConditionalBeginDestroy();
}
