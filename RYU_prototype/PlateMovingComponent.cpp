// Fill out your copyright notice in the Description page of Project Settings.

#include "PlateMovingComponent.h"
#include "GameFramework/Actor.h"
#include "Runtime/Core/Public/Containers/Array.h"


// Sets default values for this component's properties
UPlateMovingComponent::UPlateMovingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MoveDistance = 20;
	MoveStep = 0.25f;
	OriginLocation = FVector(0, 0, 30);
	MoveDown = false;
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
	/*
	if (PressurePlateMeshComp != nullptr)
	{
		FTransform trans = PressurePlateMeshComp->GetRelativeTransform();
		OriginLocation = trans.GetLocation();
	}
	*/
	
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
	//AActor* meshC = GetOwner();
	//FVector const trans = PressurePlateMeshComp->GetActorLocation();
	FTransform trans = PressurePlateMeshComp->GetRelativeTransform();
	//UE_LOG(LogTemp, Warning, TEXT("GOT IT: %s"),*trans.ToString());
	
	//UE_LOG(LogTemp, Display, TEXT("Owner is %s and located %s"), *meshC->GetName(), *trans.ToString());
	PressurePlateMeshComp->SetRelativeLocation(FVector(trans.GetLocation().X, trans.GetLocation().Y, trans.GetLocation().Z - MoveStep));
	

	
}

void UPlateMovingComponent::MovePlateUp()
{
	/*
	AActor* meshC = GetOwner();
	FVector const trans = meshC->GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("Owner is %s and located %s"), *meshC->GetName(), *trans.ToString());
	meshC->SetActorLocation(FVector(trans.X, trans.Y, trans.Z + MoveStep));
	*/
	FTransform trans = PressurePlateMeshComp->GetRelativeTransform();
	PressurePlateMeshComp->SetRelativeLocation(FVector(trans.GetLocation().X, trans.GetLocation().Y, trans.GetLocation().Z + MoveStep));

}

