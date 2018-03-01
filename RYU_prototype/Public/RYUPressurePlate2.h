// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RYUPressurePlate2.generated.h"

class UBoxComponent;
class USphereComponent;

UCLASS()
class RYU_PROTOTYPE_API ARYUPressurePlate2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARYUPressurePlate2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Components")
		UBoxComponent* TriggerZone;

	UPROPERTY(EditAnywhere, Category = "Components")
		USphereComponent* TriggerSphere;

	
	
};