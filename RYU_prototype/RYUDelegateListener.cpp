// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUDelegateListener.h"
#include "RYUGameMode.h"
#include "Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


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
	auto TheWorld = (UObject*)GetWorld();

	if (TheWorld != nullptr)
	{
		AGameModeBase *GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ARYUGameMode *RYUGameMode = (ARYUGameMode*)(GameMode);
		if (RYUGameMode != nullptr)
		{
			RYUGameMode->RYUStandardDelegate.BindUObject(this, &ARYUDelegateListener::EnableLight);
			UE_LOG(LogTemp, Display, TEXT("GameMode: %s active"), *GameMode->GetName());
		}
		
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

