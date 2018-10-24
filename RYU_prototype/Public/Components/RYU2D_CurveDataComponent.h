// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RYU2D_CurveDataComponent.generated.h"

class UCurveFloat;
class UCurveVector;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RYU_PROTOTYPE_API URYU2D_CurveDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URYU2D_CurveDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* JumpUpFloatCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveVector* JumpForwardVectorCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveVector* ClimbUpVectorCurve;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	FVector StartTimelineLocation;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	FVector EndTimelineLocation;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	float ClimbUpOffsetX;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	float ClimbUpOffsetZ;
		
	
};
