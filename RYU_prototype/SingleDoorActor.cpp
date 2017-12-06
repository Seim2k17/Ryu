// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleDoorActor.h"
#include "RYUGameMode.h"
#include "Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values
ASingleDoorActor::ASingleDoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DoorIsOpen = false;
}

// Called when the game starts or when spawned
void ASingleDoorActor::BeginPlay()
{
	Super::BeginPlay();
	
	
	auto TheWorld = (UObject*)GetWorld();

	if (TheWorld != nullptr)
	{
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ARYUGameMode* RYUGameMode = (ARYUGameMode*)(GameMode);
		if (RYUGameMode != nullptr)
		{
			OpenCloseDoorDelegate = RYUGameMode->RYUOpenCloseDoorDelegate.AddUObject(this, &ASingleDoorActor::OpenCloseDoor);
		}
	}
	
}

// Called every frame
void ASingleDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASingleDoorActor::OpenCloseDoor()
{
	if (DoorIsOpen)
	{
		UE_LOG(LogTemp, Display, TEXT("A door opens somewhere"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("A door closes somewhere"));
	}
}

void ASingleDoorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	auto TheWorld = (UObject*)GetWorld();

	if (TheWorld != nullptr)
	{
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
		ARYUGameMode* RYUGameMode = (ARYUGameMode*)(GameMode);
		if (RYUGameMode != nullptr)
		{
			RYUGameMode->RYUOpenCloseDoorDelegate.Remove(OpenCloseDoorDelegate);
		}
	}
}

