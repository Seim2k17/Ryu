// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"
#include "RYUGameMode.h"
#include "RYUDelegateListener.generated.h"

UCLASS()
class RYU_PROTOTYPE_API ARYUDelegateListener : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARYUDelegateListener();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void EnableLight();



protected:
	//ARYUDelegateListener();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY()
		UPointLightComponent* PointLight;

	
	
};
