// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUPressurePlate.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"


// Sets default values
ARYUPressurePlate::ARYUPressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");

	BorderMesh = CreateDefaultSubobject<UStaticMeshComponent>("BorderMesh");
	PressurePlateMesh = CreateDefaultSubobject<UStaticMeshComponent>("PressurePlateMesh");
		
	MovingPlateComp = CreateDefaultSubobject<UPlateMovingComponent>("MovingComponent");

	//TriggerZone = CreateDefaultSubobject<UBoxComponent>("Triggerzone");

	auto PressurePlateMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/3d_model/blockOut/blockOut_Pressure_plate_mond.blockOut_Pressure_plate_mond'"));
	auto BorderMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/3d_model/blockOut/blockOut_CutOutPlate.blockOut_CutOutPlate'"));
	

	if (PressurePlateMeshAsset.Object != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PRESSUREPLATEMESHASSET SET."))
		PressurePlateMesh->SetStaticMesh(PressurePlateMeshAsset.Object);
				
	}

	if (BorderMeshAsset.Object != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BORDERMESHASSET SET."));
		BorderMesh->SetStaticMesh(BorderMeshAsset.Object);
	}

	
	
	/*
	if (Symbol.GetValue == "Moon")
	{
		auto PressurePlateMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/3d_model/blockOut/blockOut_Pressure_plate_mond.blockOut_Pressure_plate_mond'"));
	}

	if (Symbol.GetValue == "Sun")
	{
		auto PressurePlateMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/3d_model/blockOut/blockOut_Pressure_plate_sonne.blockOut_Pressure_plate_sonne'"));
	}
*/
}

// Called when the game starts or when spawned
void ARYUPressurePlate::BeginPlay()
{
	Super::BeginPlay();
	BorderMesh->SetRelativeLocation(FVector(0, 0, 0));
	PressurePlateMesh->SetRelativeLocation(MovingPlateComp->PositionPlateOffset);
	/*
	if (TriggerZone != nullptr)
	{
		TriggerZone->SetRelativeLocation(FVector(0, 0, 0));
	}
	*/
	
			
}

// Called every frame
void ARYUPressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARYUPressurePlate::NotifyActorBeginOverlap(AActor* otherActor)
{
	++MovingPlateComp->ActorsOnPlate;
	UE_LOG(LogTemp, Display, TEXT("Actor entered Triggerzone: %s"), *otherActor->GetName());
	if (MovingPlateComp->ActorsOnPlate == 1)
	{
		MovingPlateComp->MoveDown = true;
		MovingPlateComp->MoveUp = false;
		MovingPlateComp->SetTriggeredActor(otherActor);
	}
	
}

void ARYUPressurePlate::NotifyActorEndOverlap(AActor* otherActor)
{
	--MovingPlateComp->ActorsOnPlate;
	UE_LOG(LogTemp, Display, TEXT("Actor left Triggerzone: %s"), *otherActor->GetName());
	if (MovingPlateComp->ActorsOnPlate == 0)
	{
		MovingPlateComp->MoveDown = false;
		MovingPlateComp->MoveUp = true;
		MovingPlateComp->SetTriggeredActor(nullptr);
	}
}



