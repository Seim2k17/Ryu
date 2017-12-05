// Fill out your copyright notice in the Description page of Project Settings.

#include "PlateMovingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "RYUPressurePlate.h"
#include "PushableBoxDelegatSymbol.h"
#include "Runtime/Core/Public/Containers/Array.h"

/* ISSUES

- as constructed the Components won´t set its relative Position to (0/0/0)
- the Triggerbox should automatically surrounds the PressurplateMesh + Offset
- only Activate when a certain Symbol is on top of the pressure plate (Sun/Moon ...)
*/


// Sets default values for this component's properties
UPlateMovingComponent::UPlateMovingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MoveDistance = 20;
	PositionPlateOffset = FVector(0,0,25);
	MoveStep = 0.25f;
	OriginLocation = FVector(0, 0, 30);
	MoveDown = false;
	MoveUp = false;
	ActivateSomething = false;
	ActorsOnPlate = 0;
	
	// ...
}


// Called when the game starts
void UPlateMovingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Crash if done in Constructor
	AActor* Owner = GetOwner();
	//OriginLocation = Owner->GetActorLocation();

	UE_LOG(LogTemp, Display, TEXT("Owner from PlateMovingComp is %s"), *Owner->GetName());

	//Set PressurePlateComponent
	TArray<UStaticMeshComponent*> StaticMeshComps;
	Owner->GetComponents<UStaticMeshComponent>(StaticMeshComps);
	
	for (int i = 0; i != StaticMeshComps.Num(); ++i)
	{
		//UE_LOG(LogTemp, Display, TEXT("StaticMeshComp %s"), *StaticMeshComps[i]->GetName());

		if (StaticMeshComps[i]->GetName() == "PressurePlateMesh")
		{
			UE_LOG(LogTemp, Warning, TEXT("GOT IT"));
			PressurePlateMeshComp = StaticMeshComps[i];
		}
	}
		
}


// Called every frame
void UPlateMovingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	
	FTransform trans = PressurePlateMeshComp->GetRelativeTransform();
	if (MoveDown)
	{
		if (trans.GetLocation().Z - MoveStep > OriginLocation.Z - MoveDistance)
		{
			MovePlateDown();
		}
	}

	if (trans.GetLocation().Z - MoveStep <= OriginLocation.Z - MoveDistance) {
		if (ActivateSomething == false)
		{
			//comp muss vorhanden sein sonst crash
			if (TriggeredActor != nullptr)
			{
				auto* comp = TriggeredActor->FindComponentByClass<UPushableBoxDelegatSymbol>();
				if (comp != nullptr)
				{
					ARYUPressurePlate* MyOwner = (ARYUPressurePlate*)this->GetOwner();
					//UE_LOG(LogTemp, Warning, TEXT("Symbol Box: %d "), (int)comp->Symbol);
					//UE_LOG(LogTemp, Warning, TEXT("Symbol Plate: %d "), (int)MyOwner->Symbol);
					if ((int)comp->Symbol == (int)MyOwner->Symbol)
					{
						ActivateSomething = true;
						UE_LOG(LogTemp, Warning, TEXT("You hear a click, inside Trigger is: %s "), *TriggeredActor->GetName());
					}
				}
			}
		
		}
	}

	if (MoveUp)
		{
			if (trans.GetLocation().Z + MoveStep < OriginLocation.Z)
			{
				MovePlateUp();
			}
		}
}

void UPlateMovingComponent::MovePlateDown()
{
	
	FTransform trans = PressurePlateMeshComp->GetRelativeTransform();
	PressurePlateMeshComp->SetRelativeLocation(FVector(trans.GetLocation().X, trans.GetLocation().Y, trans.GetLocation().Z - MoveStep));
	
}

void UPlateMovingComponent::MovePlateUp()
{
	
	FTransform trans = PressurePlateMeshComp->GetRelativeTransform();
	PressurePlateMeshComp->SetRelativeLocation(FVector(trans.GetLocation().X, trans.GetLocation().Y, trans.GetLocation().Z + MoveStep));

}

void UPlateMovingComponent::SetTriggeredActor(AActor* otherActor)
{
	TriggeredActor = otherActor;
}

AActor* UPlateMovingComponent::GetTriggeredActor()
{
	return TriggeredActor;
}