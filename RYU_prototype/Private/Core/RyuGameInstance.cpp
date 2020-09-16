// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuGameInstance.h"
#include "Enums/ERyuCharacterState.h"
#include "Kismet/GameplayStatics.h"
#include "Savegame/RyuCheckpoint.h"
#include "Savegame/RyuSaveGame.h"
#include "RYU_prototype.h"

void URyuGameInstance::SaveGame(ERyuCharacterState CharacterStateToSave,
                                ARyuCheckpoint* CheckpointToSave)
{
    if (URyuSaveGame* SaveGameInstance =
            Cast<URyuSaveGame>(UGameplayStatics::CreateSaveGameObject(URyuSaveGame::StaticClass())))
    {
        UE_LOG(LogRyu, Log, TEXT("Calling SaveGame"));

        // Set up the (optional) delegate.
        FAsyncSaveGameToSlotDelegate SavedDelegate;
        // USomeUObjectClass::SaveGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, bool bSuccess
        // TODO: why this no work
        SavedDelegate.BindUObject(SaveGameSuccesPointer,
                                  &URyuGameInstance::SaveGameSuccessCallback);

        // Set data on the savegame object.
        SaveGameInstance->CharacterState = CharacterStateToSave;
        SaveGameInstance->Checkpoint = CheckpointToSave;
        SaveGameInstance->CheckpointLocation = CheckpointToSave->GetActorLocation();
        SaveGameInstance->PlayerName = TEXT("Ryudude");

        // Start async save process.
        UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName,
                                              SaveGameInstance->UserIndex, SavedDelegate);
    }
}

void URyuGameInstance::LoadGame()
{
    UE_LOG(LogRyu, Log, TEXT("Loading Savinggame."));
    FString SlotName = TEXT("TestSaveSlot");
    /* Asynchronous Loading does not work ?
	// Set up the delegate.
    FAsyncLoadGameFromSlotDelegate LoadedDelegate;
    // USomeUObjectClass::LoadGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData
    LoadedDelegate.BindUObject(LoadGameSuccesPointer, &URyuGameInstance::LoadGameSuccessCallback);
	
    UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadedDelegate);
	*/

    // Retrieve and cast the USaveGame object to UMySaveGame.
    if (URyuSaveGame* LoadedGame =
            Cast<URyuSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
    {
        // The operation was successful, so LoadedGame now contains the data we saved earlier.
        UE_LOG(LogTemp, Warning, TEXT("LOADED: %s at %s"), *LoadedGame->PlayerName,
               *(LoadedGame->CheckpointLocation).ToString());
        OnGameLoaded.Broadcast(LoadedGame->CheckpointLocation);
    }
}

void URyuGameInstance::SaveGameSuccessCallback(const FString& SlotName, const int32 UserIndex,
                                               bool bSuccess)
{
    UE_LOG(LogRyu, Log, TEXT("Saveing the Game was success: %b"), bSuccess);
}

void URyuGameInstance::LoadGameSuccessCallback(const FString& SlotName, const int32 UserIndex,
                                               USaveGame* LoadedGameData)
{
    UE_LOG(LogRyu, Log, TEXT("Game loaded"));
}
