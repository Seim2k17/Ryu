// Fill out your copyright notice in the Description page of Project Settings.

#include "WalkSplineComponent.h"
#include "Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "RYUGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
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
		UE_LOG(LogTemp, Display, TEXT("Spline Found as Component in Actor: %s"), *GetOwner()->GetName());
		auto* world = GetWorld();
		if (world != nullptr)
		{
			AGameModeBase* gm = UGameplayStatics::GetGameMode(world);
			ARYUGameMode* RYUGameMode = (ARYUGameMode*)(gm);
			if (RYUGameMode != nullptr)
			{
				// #AlignChar
				RYUGameMode->RYUEnterSplineCollider.AddDynamic(this, &UWalkSplineComponent::setCharacterToSpline);
				getReferences();

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spline not found in Actor: %s"), *GetOwner()->GetName());
		}
	}
	// ...
	
}


// Called every frame
void UWalkSplineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// #AlignChar
void UWalkSplineComponent::setCharacterToSpline()
{

	if (connectedSpline != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Character blabla is set to Spline %s"), *connectedSpline->GetName());
	}
}

void UWalkSplineComponent::getReferences()
{
	// #References
	//setting Reference to mainchar
	auto* world = GetWorld();
	MyCharacter = UGameplayStatics::GetPlayerPawn(world,0);
	UE_LOG(LogTemp, Display, TEXT("Character %s is set"), *MyCharacter->GetName());


	//if (MyCharacter->WalkSplineActive)
	{
		UE_LOG(LogTemp, Display, TEXT("Walk on Splines is set to active!"));
	}
}

