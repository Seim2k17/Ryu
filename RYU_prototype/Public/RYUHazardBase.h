// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PainCausingVolume.h"
#include "RYUHazardBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RYU_PROTOTYPE_API ARYUHazardBase : public APainCausingVolume
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ARYUHazardBase();

protected:
	

public:
	

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* Spikes;
	
	
};
