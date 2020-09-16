// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "Enums/ERyuCharacterState.h"
#include "CoreMinimal.h"
#include "RyuGameInstance.generated.h"

class ARyuCheckpoint;
class USaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameLoadedDelegate, FVector&, CheckpointPosition);

UCLASS()
class RYU_PROTOTYPE_API URyuGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = Basic)
    void SaveGame(ERyuCharacterState CharacterStateToSave, ARyuCheckpoint* CheckpointToSave);

    UFUNCTION(BlueprintCallable, Category = Basic)
    void LoadGame();

    UFUNCTION()
    void SaveGameSuccessCallback(const FString& SlotName, const int32 UserIndex, bool bSuccess);

    UFUNCTION()
    void LoadGameSuccessCallback(const FString& SlotName, const int32 UserIndex,
                                 USaveGame* LoadedGameData);

    UPROPERTY()
    URyuGameInstance* SaveGameSuccePointer;

    UPROPERTY()
    URyuGameInstance* LoadGameSuccesPointer;

    UPROPERTY()
    URyuGameInstance* SaveGameSuccesPointer;

    UPROPERTY()
    FOnGameLoadedDelegate OnGameLoaded;
};
