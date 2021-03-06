// Fill out your copyright notice in the Description page of Project Settings.

#include "PushableBoxDelegatSymbol.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"


// Sets default values for this component's properties
UPushableBoxDelegatSymbol::UPushableBoxDelegatSymbol()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	sizeX = 0;
	sizeY = 0;
	sizeZ = 0;
	// ...
}


// Called when the game starts
void UPushableBoxDelegatSymbol::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UPushableBoxDelegatSymbol::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

