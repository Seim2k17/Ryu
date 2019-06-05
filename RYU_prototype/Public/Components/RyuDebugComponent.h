// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "RyuMainCharacter.h"
#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "RyuDebugComponent.generated.h"

class ARyuMainCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RYU_PROTOTYPE_API URyuDebugComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URyuDebugComponent();

    void TickComponent(float DeltaTime, ELevelTick TickType,
                       FActorComponentTickFunction* ThisTickFunction) override;

    void PostEditChangePropertyFromOwner();

    void DrawDebugInfosOnScreen();

protected:
    void BeginPlay() override;

private:
    // we can activate/deactivate Debugging
    void InitDebugValues();

    // TODO Move To a Static UtilityFunctionClass
    ARyuMainCharacter* GetMainCharOwner();

    UPROPERTY(EditAnywhere, Category = "Preferences")
    bool bActivated = false;
};
