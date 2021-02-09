// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuStaticFunctionLibrary.h"
#include "Enums/ERyuCharacterPossibility.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuMoveRightAxisInputState.h"
#include "Enums/ERyuMovementState.h"
#include "RYU_prototype.h"
#include "RyuMainCharacter.h"
#include <Components/ActorComponent.h>

URyuStaticFunctionLibrary::URyuStaticFunctionLibrary(const FObjectInitializer& Objectinitializer)
    : Super(Objectinitializer)
{
}

ARyuMainCharacter* URyuStaticFunctionLibrary::GetMainChar(AActor* actor)
{
    return Cast<ARyuMainCharacter>(actor);
}

ARyuMainCharacter* URyuStaticFunctionLibrary::GetMainCharOwner(UActorComponent* Component)
{
    return Cast<ARyuMainCharacter>(Component->GetOwner());
}

FString URyuStaticFunctionLibrary::CharacterStateToString(const ERyuCharacterState State)
{
    return EnumToString(TEXT("ERyuCharacterState"), State, TEXT("ERyuCharacterState::Invalid"));
}

FString URyuStaticFunctionLibrary::InputStateToString(const ERyuInputState State)
{
    return EnumToString(TEXT("ERyuInputState"), State, TEXT("ERyuInputState::Invalid"));
}

FString URyuStaticFunctionLibrary::MovementStateToString(const ERyuMovementState State)
{
    return EnumToString(TEXT("ERyuMovementState"), State, TEXT("ERyuMovementState::Invalid"));
}

FString URyuStaticFunctionLibrary::RightAxisInputStateToString(
    const ERyuMoveRightAxisInputState State)
{
    return EnumToString(TEXT("ERyuMoveRightAxisInputState"), State,
                        TEXT("ERyuMoveRightAxisInputState::Invalid"));
}

FString URyuStaticFunctionLibrary::CharacterPossibilityToString(
    const ERyuCharacterPossibility Possibility)
{
    return EnumToString(TEXT("ERyuCharacterPossibility"), Possibility,
                        TEXT("ERyuCharacterPossibility::Invalid"));
}

// TODO: StaticLog
void URyuStaticFunctionLibrary::Log(FString Message, FName Category)
{
    UE_LOG(LogRyu, Log, TEXT(""));
}

template <typename T>
bool URyuStaticFunctionLibrary::CheckIfActorIsInSet(T& Actor, TSet<AActor*>& Datastructure)
{
    bool isInside = false;
    for (auto actor : Datastructure)
    {
        if (actor == Actor)
        {
            isInside = true;
            break;
        }
    }

    return isInside;
}
