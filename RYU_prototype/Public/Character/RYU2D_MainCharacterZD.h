// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RYU2D_CharacterBase.h"
#include <CoreMinimal.h>
#include <Components/TimelineComponent.h>
#include "RYU2D_MainCharacterZD.generated.h"

class URYU2D_MovementComponent;
class URYU2D_CurveDataComponent;
class UPaperZDAnimBP;

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

    void PostInitializeComponents() override;

    void Jump() override;

	UFUNCTION(BlueprintCallable, Category = "Jumping")
	void JumpForward();

    void StopJumping() override;

    void Tick(float DeltaTime) override;

    void DrawDebugInfosOnScreen();

    void DebugSomething();

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void ChangeMovementMode();

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void ChangeClimbingMode();

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    //Public due Customized CharMovementComponent
    void Climb();

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

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void TurnRunFlipBookFinished();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ClimbLedgeFlipBookFinished();

    UFUNCTION(BlueprintCallable, Category = "ClimbTrigger")
    bool CheckFlipOverlappedActor(UBoxComponent* ClimbingTrigger);

    UFUNCTION(BlueprintCallable)
    UBoxComponent* GetOverlappedClimbingComponent(FName UpOrDown, FName LeftOrRight);

    UBoxComponent* GetOverlappedClimbingComponent(ERYULedgePosition2D LedgePosi);

    UFUNCTION(BlueprintCallable, Category = "Jumping")
    void StartLaunchCharacter();

protected:
    void BeginPlay() override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** Called for side to side input */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveRight(float Val);

    /** Called for Climbing input */
    void MoveUp(float Value);

    UFUNCTION()
    void HandleSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                          AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                          int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult);

    UFUNCTION()
    void HandleSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex);

    /** Start-TIMELINE-SECTION */
    //Updatefunction for the Timeline
    UFUNCTION()
    void TimelineCallbackFloat(float val);

    UFUNCTION()
    void TimelineCallbackVector(FVector Vec);

    //When Finished the Timeline
    UFUNCTION()
    void TimelineFinishedCallback();

    void PlayTimeline();

    void SetCurrentTimelineParamsFloat(UCurveFloat* FloatCurveX, UCurveFloat* FloatCurveZ,
                                       bool TimelineIsLooping, bool IgnoreTimeDilation);

    void SetCurrentTimelineParamsVector(UCurveVector* VectorCurve, bool TimelineIsLooping,
                                        bool IgnoreTimeDilation);

    /*onTimelineCallback contains the signature of the function that is
	going to execute every time we tick our timeline.
	Think of onTimelineCallback as a delegate!*/
    /** Declare our delegate function to be binded with TimelineCallback */
    FOnTimelineFloat onTimelineCallbackFloat{};

    FOnTimelineVector onTimelineCallbackVector{};

    //FOnTimelineEventStatic onTimelineFinishedCallback;

    /** Declare our delegateFunction to be binded to TimelineFinishedCallback()*/
    FOnTimelineEvent onTimelineFinishedCallback{};

    /** END-TIMELINE-SECTION */

    /* Climbing Methods*/

    void HangOnLedgeAndClimb(float Val);

    void CanGrabLedges(float Val);

    void JumpUpOrDown(float Val, FVector StartJumpPosition);

    void UpdateCharacter();

    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetMoveRightInput();

    UFUNCTION()
    void SneakPressed();

    UFUNCTION()
    void SneakReleased();

    void SetClimbingPostitionsAndMovementMode(EPlayerMovement PlayerMove,
                                              UBoxComponent* ClimbingTrigger);

    void SetClimbingPositions(UBoxComponent* ClimbTrigger);

private:
    void InitializeCharacterValues();

    void CheckMoveUpState();

    void ResetClimbingState();

    /************************************************************************/
    /* MEMBER                                                               */
    /************************************************************************/

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SneakMultiplier;

protected:
    bool bPlayTurnAni;

    bool bStartedNoLoopAnimation;

    //** TimeLine Stuff */
    UPROPERTY()
    TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

    UPROPERTY()
    UTimelineComponent* CurrentTimeline;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    URYU2D_CurveDataComponent* Curve2DComponent;

    UPROPERTY(EditAnywhere, Category = "Jumping")
    FVector JumpImpulse = FVector::ZeroVector;

private:
    float fDeltaSeconds;

    float bDebugOutputActive;

    FVector currA;
    FVector currV;

    //**MOVEMENT*
    //now in BaseClass !
    //bool bLookRight;

    //*ClilmbUpVariables
    bool bHangPositionSet;

    FVector _StartClimbUpPosition;

    float MoveRightInput;

    float MoveUpInput;

    float SneakMultiplierValue;

    bool bSneakIsPressed;
};
