// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SingleDoorActor.generated.h"

UCLASS()
class RYU_PROTOTYPE_API ASingleDoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASingleDoorActor();

	FDelegateHandle OpenCloseDoorDelegate;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OpenCloseDoor();
	//UFUNCTION()
	//	virtual void EndPlay(constEEndPlayReason::Type EndPlayReason) override;

private:
	bool DoorIsOpen;

	
	
};
