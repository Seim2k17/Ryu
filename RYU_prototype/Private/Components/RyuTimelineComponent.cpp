// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuTimelineComponent.h"

// Sets default values for this component's properties
URyuTimelineComponent::URyuTimelineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void URyuTimelineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URyuTimelineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
