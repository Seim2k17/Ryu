// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUHazardBase.h"

ARYUHazardBase::ARYUHazardBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spikes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
	RootComponent = Spikes;

	


}


