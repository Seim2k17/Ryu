// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RYUCustomizeMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RYU_PROTOTYPE_API URYUCustomizeMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URYUCustomizeMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "CharacterMovement")
		void SetGravityScaleMaximum(float GravScale);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		float AddFallingMultiplierNumber;

	/**if 0 then Char->GravityMaximumStd (3) is used*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		float GravityScaleMaximum;

	/** Jump Force added when pressing Jump*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Customization)")
		FVector JumpForce;
		
	
};
