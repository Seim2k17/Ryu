// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RYU2D_CharacterBase.h"
#include "Components/TimelineComponent.h"
#include "RYU2D_CharacterPrince.generated.h"

//@ToDo: look later when movement impl.
class URYU2D_MovementComponent;
class URYU2D_AnimationComponent;
class UPaperFlipbook;


/**
 * 
 */
UCLASS()
class RYU_PROTOTYPE_API ARYU2D_CharacterPrince : public ARYU2D_CharacterBase
{
	GENERATED_BODY()
	
/************************************************************************/
/* METHODS                                                              */
/************************************************************************/

public:
	
	ARYU2D_CharacterPrince(const class FObjectInitializer& ObjectInitializer);


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

protected:

	void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float Val);

	/** Called for Climbing input */
	void MoveUp(float Value);

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	void UpdateCharacter();

	UFUNCTION()
	void FlipbookFinishedPlaying();

	void BeginRunFlipbookFinished();

	void EndRunFlipbookFinished();

	void StartTurnFlipbookFinished();

	void ClimbingFlipbookFinished();

	void PlayFlipBookAnimation(UPaperFlipbook* AnimationToPlay, bool bAnimationIsLooping);

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

private:

	void InitializeCharacterValues();

/************************************************************************/
/* MEMBER                                                               */
/************************************************************************/

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector TurnPositionOffset;

protected:


	bool bPlayTurnAni;

	bool bStartedNoLoopAnimation;

	//**MOVEMENT*
	bool bLookRight;

	//** TimeLine Stuff */
	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	UPROPERTY()
	UTimelineComponent* CurrentTimeline;

	/**ANIMATIONS as extra Component*/

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URYU2D_AnimationComponent* Animation2DComponent;

private:

	float fDeltaSeconds;

	float bDebugOutputActive;

	FVector currA;
	FVector currV;

	//*ClilmbUpVariables
	bool bHangPositionSet;

	FVector _StartClimbUpPosition;

};
