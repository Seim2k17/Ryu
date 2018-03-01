// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUPressurePlate2.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ARYUPressurePlate2::ARYUPressurePlate2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	RootComponent = TriggerZone;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(TriggerZone);

}

// Called when the game starts or when spawned
void ARYUPressurePlate2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARYUPressurePlate2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

