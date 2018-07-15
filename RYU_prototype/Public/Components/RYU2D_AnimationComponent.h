// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RYU2D_AnimationComponent.generated.h"

class UPaperFlipbook;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RYU_PROTOTYPE_API URYU2D_AnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URYU2D_AnimationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	/**linked Animations */

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* RunningAnimation;
	
	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimation;

	// The animation to play when Start Running
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* StartRunAnimation;

	// The animation to play when Start Running
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* EndRunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* TurnAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpUpAndFallAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpUpAndHangAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* HangOnLedgeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* ClimbUpLedgeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* ClimbDownLedgeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* FallingFromLedgeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* FallingLoopAnimation;

	/**and resp. Curves to some Animations */

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* JumpUpFloatCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* ClimbUpFloatCurve;
	
	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	FVector ClimbUpStartTimelineLocation;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	FVector ClimbUpEndTimelineLocation;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	float ClimbUpOffset;

};
