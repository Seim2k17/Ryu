// Copyright 2019 80k Games, All Rights Reserved.

#include "RyuCurveDataComponent.h"

/* Stuff from CurveDCata will go into the RyuTimeLineComponent !!! */

// Sets default values for this component's properties
URyuCurveDataComponent::URyuCurveDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URyuCurveDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URyuCurveDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

