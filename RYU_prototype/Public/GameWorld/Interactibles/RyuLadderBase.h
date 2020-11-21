// Copyright 2019 80k Games, All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "RyuLadderBase.generated.h"

UCLASS()
class RYU_PROTOTYPE_API ARyuLadderBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARyuLadderBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions", Meta = (MakeEditWidget = true))
    FVector StartClimbingPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positions", Meta = (MakeEditWidget = true))
    FVector ExitClimbingPosition;
};
