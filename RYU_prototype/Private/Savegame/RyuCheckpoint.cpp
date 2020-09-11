// Copyright 2019 80k Games, All Rights Reserved.


#include "RyuCheckpoint.h"

// Sets default values
ARyuCheckpoint::ARyuCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARyuCheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARyuCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

