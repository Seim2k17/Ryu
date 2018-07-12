// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RYU2D_MovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class RYU_PROTOTYPE_API URYU2D_MovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
/************************************************************************/
/* METHODS                                                              */
/************************************************************************/

public:

	URYU2D_MovementComponent();

	URYU2D_MovementComponent(const class FObjectInitializer& ObjectInitializer);


/************************************************************************/
/* MEMBER                                                               */
/************************************************************************/

public:

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float TreshholdYWalkRun;
	
	
};
