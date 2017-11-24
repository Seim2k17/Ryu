// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUDelegateListener.h"
#include "RYUGameMode.h"


// Sets default values
ARYUDelegateListener::ARYUDelegateListener()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLight = CreateDefaultSubobject<UPointLightComponent>("pointLight");
	RootComponent = PointLight;
	PointLight->SetVisibility(false);
}

// Called when the game starts or when spawned
void ARYUDelegateListener::BeginPlay()
{
	Super::BeginPlay();
	UWorld* TheWorld = GetWorld();

	if (TheWorld != nullptr)
	{
		//UGameMode *GameMode = UGameplayStatistics::GetGameMode(TheWorld);
	}
}

// Called every frame
void ARYUDelegateListener::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARYUDelegateListener::EnableLight()
{

}

