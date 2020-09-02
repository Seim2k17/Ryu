// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterFallingState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMovementState.h"
#include "Utilities/RyuStaticFunctionLibrary.h"
#include "RYU_prototype.h"
#include "RyuBaseCharacter.h"
#include "RyuCharacterFallingEndState.h"
#include "RyuMainCharacter.h"

URyuCharacterFallingState::URyuCharacterFallingState()
{
}

URyuCharacterState* URyuCharacterFallingState::HandleInput(ARyuBaseCharacter* Character,
                                                           const ERyuInputState Input)
{
    UE_LOG(LogRyu, Log, TEXT("HandleInput from FallingState: %s"),
           *URyuStaticFunctionLibrary::InputStateToString(Input));
    // only make special call when Input occurs which is not in the Baseclass, otherwise we don´t need to handle Input, just walk up in the hierarchy
    switch (Input)
    {
        case ERyuInputState::InputEndFalling:
        {
            //CharacterState = ERyuCharacterState::JumpEnd;
            return NewObject<URyuCharacterFallingEndState>();
        }

        default:
            return Super::HandleInput(Character, Input);
            break;
    }
}

void URyuCharacterFallingState::Update(ARyuBaseCharacter* Character)
{
	UE_LOG(LogRyu, Warning, TEXT("RyuCharacterFallingState(Update):"));
    if (auto* MainChar = Cast<ARyuMainCharacter>(Character))
    {
        Super::Update(Character);
        FallDownSpeed = Character->GetVelocity().Z;
        if (FallDownSpeed < Character->GetFallDeepVelocityZ())
        {
            CharacterState = ERyuCharacterState::FallingDeep;
        }

        if (FallDownSpeed <= Character->GetFallToDeathVelocityZ())
        {
            //CharacterState = ERyuCharacterState::FallingDeath;
        }

        FHitResult TraceHit = MainChar->GetHitResult();
        UE_LOG(LogRyu, Warning, TEXT("RyuCharacterFallingState(Update): %s"), *TraceHit.ToString());
        if (TraceHit.bBlockingHit)
        {
            // TODO: differ fallingendstate if falling normal/deep or DIE
            Character->HandleInput(ERyuInputState::InputEndFalling);
        }
    }
}

void URyuCharacterFallingState::Enter(ARyuBaseCharacter* Character)
{
    Character->JumpToAnimInstanceNode(Character->FallinghNodeName);
    CharacterState = ERyuCharacterState::FallingNormal;
    FallDownSpeed = Character->GetVelocity().Z;
}

void URyuCharacterFallingState::Exit(ARyuBaseCharacter* Character)
{
}
