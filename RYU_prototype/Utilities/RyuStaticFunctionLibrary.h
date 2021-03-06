// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Enums/ERyuCharacterPossibility.h"
#include "Enums/ERyuCharacterState.h"
#include "Enums/ERyuInputState.h"
#include "Enums/ERyuLogCategory.h"
#include "Enums/ERyuMoveRightAxisInputState.h"
#include "Enums/ERyuMovementState.h"
#include "RyuStaticFunctionLibrary.generated.h"

class ARyuMainCharacter;
class UActorComponent;

template<typename T>
FString EnumToString(const FString& EnumName, const T Value, const FString& DefaultValue)
{
    UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
    if (!Enum)
    {
        return DefaultValue;
    }

    FString Name = Enum->GetNameByIndex(static_cast<uint8>(Value)).ToString();
    return Name.RightChop(1).TrimStart().TrimEnd();
}

UCLASS()
class RYU_PROTOTYPE_API URyuStaticFunctionLibrary : public UObject
{
    GENERATED_UCLASS_BODY()
public:
    //FORCEINLINE function

    static ARyuMainCharacter* GetMainChar(AActor* actor);

    static ARyuMainCharacter* GetMainCharOwner(UActorComponent* Component);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    static FString CharacterStateToString(const ERyuCharacterState State);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    static FString InputStateToString(const ERyuInputState State);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    static FString MovementStateToString(const ERyuMovementState State);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    static FString RightAxisInputStateToString(const ERyuMoveRightAxisInputState State);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    static FString CharacterPossibilityToString(const ERyuCharacterPossibility Possibility);

    static void Log(FString Message, FName Category);

    template <typename T>
    static bool CheckIfActorIsInSet(T&, TSet<AActor*>&);
};
