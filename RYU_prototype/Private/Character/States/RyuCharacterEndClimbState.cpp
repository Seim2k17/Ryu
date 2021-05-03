// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCharacterEndClimbState.h"
#include "Character/RyuMainCharacter.h"
#include "Enums/ERyuCharacterState.h"
#include "States/RyuCharacterIdleState.h"

URyuCharacterEndClimbState::URyuCharacterEndClimbState()
{
}

URyuCharacterState* URyuCharacterEndClimbState::HandleInput(ARyuBaseCharacter* Character,
                                                            const ERyuInputState Input)
{
    switch (Input)
    {
        case ERyuInputState::AnimationEnded:
        {
            UE_LOG(LogRyu, Log, TEXT("ClimbingEndState: AnimationEnded, jump to Idle: "));
            UE_LOG(LogRyu, Log, TEXT("ClimbingEndState: Character SetOnLocation: "));
            // TODO: set Character to LadderObject top or DownPosition
            if (auto MainChar = Cast<ARyuMainCharacter>(Character))
            {
                switch (ClimbOutState)
                {
                    case EClimboutState::Top:
                    {
                        MainChar->SetActorLocation(ClimbOutTop);
                        UE_LOG(LogRyu, Log, TEXT("ClimbingEndState: OnTop at: %s"),
                               *ClimbOutTop.ToString());
                        break;
                    }

                    case EClimboutState::Bottom:
                    {
                        MainChar->SetActorLocation(ClimbOutBtm);
                        UE_LOG(LogRyu, Log, TEXT("ClimbingEndState: OnBtm at: %s"),
                               *ClimbOutBtm.ToString());
                        break;
                    }
                    default:
                        break;
                }

                MainChar->GetRyuCharacterMovement()->ResetClimbingState();
                MainChar->SetClimbPossibility();
            }

            return NewObject<URyuCharacterIdleState>();
        }

        default:
        {
            return Super::HandleInput(Character, Input);
            break;
        }
    }
}

void URyuCharacterEndClimbState::Update(ARyuBaseCharacter* Character)
{
}

void URyuCharacterEndClimbState::Enter(ARyuBaseCharacter* Character)
{
    UE_LOG(LogRyu, Log, TEXT("ClimbingEndState: ENTER: "));
    CharacterState = ERyuCharacterState::ExitLadder;

    //Character->JumpToAnimInstanceNode(Character->IdleNodeName);

    /* ToDo: Check which Climbing-Type end shere !
    switch (CharacterState)
    {
        case ERyuCharacterState::
        case ERyuCharacterState::ClimbDownLadder:
        {
            UE_LOG(LogRyu, Log, TEXT("ClimbingEndState: EXIT LADDER: "));
            CharacterState = ERyuCharacterState::ExitLadder;
            break;
        }
        default:
            break;
    }
    */
}

void URyuCharacterEndClimbState::Exit(ARyuBaseCharacter* Character)
{
}

URyuCharacterEndClimbState* URyuCharacterEndClimbState::MAKE(EClimboutState ClimbingOutState,
                                                             FVector ClimbingOutTop,
                                                             FVector ClimbingOutBtm)
{
    URyuCharacterEndClimbState* obj = NewObject<URyuCharacterEndClimbState>();
    obj->SetClimbOutState(ClimbingOutState, ClimbingOutTop, ClimbingOutBtm);
    return obj;
}

void URyuCharacterEndClimbState::SetClimbOutState(EClimboutState ClimbingOutState,
                                                  FVector ClimbingOutTop, FVector ClimbingOutBtm)
{
    ClimbOutState = ClimbingOutState;
    ClimbOutTop = ClimbingOutTop;
    ClimbOutBtm = ClimbingOutBtm;
}
