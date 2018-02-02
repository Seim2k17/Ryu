// Fill out your copyright notice in the Description page of Project Settings.

#include "WalkSplineComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"


// Sets default values for this component's properties
UWalkSplineComponent::UWalkSplineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UWalkSplineComponent::BeginPlay()
{
	Super::BeginPlay();
	

	connectedSpline = GetOwner()->FindComponentByClass(USplineComponent::StaticClass());
	if (connectedSpline != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Spline Found as Component in Actor: %s"),*GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spline not found in Actor: %s"),*GetOwner()->GetName());
	}
	// ...
	
}


// Called every frame
void UWalkSplineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

