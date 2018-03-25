// Fill out your copyright notice in the Description page of Project Settings.

#include "RYUHazard_Spikes.h"
#include "Components/BoxComponent.h"



// Sets default values
ARYUHazard_Spikes::ARYUHazard_Spikes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spikes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
	RootComponent = Spikes;

	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerArea->SetupAttachment(Spikes);
	TriggerArea->SetBoxExtent(FVector(50,50,100));

	//what happen when overlapping
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ARYUHazard_Spikes::SpikesBeginOverlap);


}

// Called when the game starts or when spawned
void ARYUHazard_Spikes::BeginPlay()
{
	Super::BeginPlay();
	
}


// 
void ARYUHazard_Spikes::SpikesBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
 	UE_LOG(LogTemp, Log, TEXT("YOU DIED"));
 	if (OtherActor != NULL)
 	{
	
 	}
}



// Called every frame
void ARYUHazard_Spikes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

