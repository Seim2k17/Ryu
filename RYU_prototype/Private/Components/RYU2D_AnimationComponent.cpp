// Fill out your copyright notice in the Description page of Project Settings.

#include "RYU2D_AnimationComponent.h"


// Sets default values for this component's properties
URYU2D_AnimationComponent::URYU2D_AnimationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	ClimbUpOffset = 10.0f;
}


// Called when the game starts
void URYU2D_AnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


