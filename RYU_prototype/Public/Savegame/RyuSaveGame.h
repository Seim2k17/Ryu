// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Enums/ERyuCharacterState.h"
#include "GameFramework/SaveGame.h"
#include "Savegame/RyuCheckpoint.h"
#include "CoreMinimal.h"
#include "RyuSaveGame.generated.h"

UCLASS()
class RYU_PROTOTYPE_API URyuSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    URyuSaveGame();

    UPROPERTY(VisibleAnywhere, Category = Basic)
    ERyuCharacterState CharacterState;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString PlayerName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    ARyuCheckpoint* Checkpoint;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FVector CheckpointLocation;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;

private:
};
