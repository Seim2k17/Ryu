// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuLadderBase.h"

// Sets default values
ARyuLadderBase::ARyuLadderBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
}

float ARyuLadderBase::GetClimboutTopTreshold()
{
    return ClimbOutTopTreshold;
}

float ARyuLadderBase::GetClimboutBtmTreshold()
{
    return ClimbOutBottomTreshold;
}

// Called when the game starts or when spawned
void ARyuLadderBase::BeginPlay()
{
    Super::BeginPlay();
}
