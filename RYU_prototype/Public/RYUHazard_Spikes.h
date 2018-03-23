// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RYUHazard_Spikes.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class RYU_PROTOTYPE_API ARYUHazard_Spikes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARYUHazard_Spikes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Spikes;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* TriggerArea;

	UPROPERTY(EditAnywhere, Category = "Preferences")
	bool bInstantDeath;

	UPROPERTY(EditAnywhere, Category = "Preferences")
	float DamagePerSecond;


	
};
