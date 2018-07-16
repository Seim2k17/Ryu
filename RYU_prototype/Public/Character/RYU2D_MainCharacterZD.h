﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RYU2D_CharacterBase.h"
#include "Components/TimelineComponent.h"
#include "RYU2D_MainCharacterZD.generated.h"


class URYU2D_MovementComponent;

/**
 * 
 */
UCLASS()
class RYU_PROTOTYPE_API ARYU2D_MainCharacterZD : public ARYU2D_CharacterBase
{
	GENERATED_BODY()
	
		/************************************************************************/
		/* METHODS                                                              */
		/************************************************************************/

public:

	ARYU2D_MainCharacterZD(const class FObjectInitializer& ObjectInitializer);


	//UPROPERTY(Category = Character, VAnywhere, BlueprintReadOnly)
	URYU2D_MovementComponent* MovementComp;

	virtual void PostInitializeComponents() override;

	void Jump() override;

	void StopJumping() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DrawDebugInfosOnScreen();

	void DebugSomething();


#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	//Public due Customized CharMovementComponent
	void Climb(float Val);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetCurrentVelocity();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetCurrentAcceleration();

	UFUNCTION(BlueprintPure, Category = "Movement")
	bool GetIsLookingRight();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetLookRight();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void TurnFlipBookFinished();

protected:

	void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float Val);

	/** Called for Climbing input */
	void MoveUp(float Value);

	
	UFUNCTION()
	void HandleSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void HandleSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Start-TIMELINE-SECTION */
	//Updatefunction for the Timeline
	UFUNCTION()
	void TimelineCallback(float val);

	//When Finished the Timeline
	UFUNCTION()
	void TimelineFinishedCallback();

	void PlayTimeline();

	void SetCurrentTimelineParams(UCurveFloat* FloatCurve, bool TimelineIsLooping, bool IgnoreTimeDilation);

	/*onTimelineCallback contains the signature of the function that is
	going to execute every time we tick our timeline.
	Think of onTimelineCallback as a delegate!*/
	/** Declare our delegate function to be binded with TimelineCallback */
	FOnTimelineFloat onTimelineCallback{};

	//FOnTimelineEventStatic onTimelineFinishedCallback;

	/** Declare our delegateFunction to be binded to TimelineFinishedCallback()*/
	FOnTimelineEvent onTimelineFinishedCallback{};

	/** END-TIMELINE-SECTION */

	/* Climbing Methods*/
	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void CanClimbUp(float Val, FVector StartClimbUpPosition);

	void CanClimbDown(float Val);

	void CanClimbUpAndDown(float Val, FVector StartClimbPosition);

	void HangOnLedgeAndClimb(float Val);

	void CanGrabLedges(float Val);

	void JumpUpOrDown(float Val, FVector StartJumpPosition);

	void UpdateCharacter();

	UFUNCTION(BlueprintPure, Category = "Movement")
	float GetMoveRightInput();

private:

	void InitializeCharacterValues();

/************************************************************************/
/* MEMBER                                                               */
/************************************************************************/

public:

	
protected:


	bool bPlayTurnAni;

	bool bStartedNoLoopAnimation;

	//** TimeLine Stuff */
	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	UPROPERTY()
	UTimelineComponent* CurrentTimeline;

private:

	float fDeltaSeconds;

	float bDebugOutputActive;

	FVector currA;
	FVector currV;

	//**MOVEMENT*
	bool bLookRight;

	//*ClilmbUpVariables
	bool bHangPositionSet;

	FVector _StartClimbUpPosition;
	
	float MoveRightInput;

	float MoveUpInput;
	
};