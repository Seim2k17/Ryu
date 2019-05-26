// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RYU2D_CharacterBase.h"
#include <CoreMinimal.h>
#include <Components/TimelineComponent.h>
#include "RYU2D_MainCharacterZD.generated.h"

class URYU2D_MovementComponent;
class URYU2D_CurveDataComponent;
class UPaperZDAnimBP;
class UPaperZDAnimPlayer;
class UPaperZDAnimSequence;

UCLASS()
class RYU_PROTOTYPE_API ARYU2D_MainCharacterZD : public ARYU2D_CharacterBase
{
    GENERATED_BODY()

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

public:
    ARYU2D_MainCharacterZD(const class FObjectInitializer& ObjectInitializer); //*copied

    //UPROPERTY(Category = Character, VAnywhere, BlueprintReadOnly)
    URYU2D_MovementComponent* MovementComp;

    void PostInitializeComponents() override; //*copied

    void Jump() override; //*copied

    void Tick(float DeltaTime) override; //*copied

    // register paperZD Stuff override !
    void ConfigurePlayer_Implementation(UPaperZDAnimPlayer* Player) override; //*copied

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override; //*copied
#endif

    //*TO COMPONENTS START
    UFUNCTION(BlueprintCallable, Category = "Jumping")
    void JumpForward();

    UFUNCTION(BlueprintCallable, Category = "Jumping")
    void JumpUp();

    void StopJumping() override;

    void DrawDebugInfosOnScreen();

    void DebugSomething();

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void ChangeMovementMode();

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void ChangeClimbingMode();

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

    //*TO COMPONENTS END

protected:
    void BeginPlay() override; //*copied

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; //* copied


	//*TO COMPONENTS START

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

    //** TimeLine Stuff */
    UPROPERTY()
    TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

    UPROPERTY()
    UTimelineComponent* CurrentTimeline;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    URYU2D_CurveDataComponent* Curve2DComponent;

    /** END-TIMELINE-SECTION */

    /* Climbing Methods - We make this a component later of course !*/

    void HangOnLedgeAndClimb(float Val);

    void CanGrabLedges(float Val);

    void SetClimbingPostitionsAndMovementMode(EPlayerMovement PlayerMove,
                                              UBoxComponent* ClimbingTrigger);

    void SetClimbingPositions(UBoxComponent* ClimbTrigger);

    // climbingmodemethod
    void CheckMoveUpState();

    void ResetClimbingState();

	 //*TO COMPONENTS END
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

    void JumpUpOrDown(float Val, FVector StartJumpPosition);  //* copied

    void UpdateCharacter();  //* copied

    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetMoveRightInput(); //* copied

    UFUNCTION()
    void SneakPressed(); //* copied

    UFUNCTION()
    void SneakReleased();//* copied



private:
    void InitializeCharacterValues(); //* copied

    // our own functionality gets called at the end of an animation, due its a delegate function we need to mark it as UFUNCTION() with InAnimSequence parmList
    UFUNCTION()
    void AnimationSequenceEnded(const UPaperZDAnimSequence* InAnimSequence);  //* copied

    /************************************************************************/
    /* MEMBER                                                               */
    /************************************************************************/

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SneakMultiplier;

protected:
    bool bPlayTurnAni;

    bool bStartedNoLoopAnimation;





    UPROPERTY(EditAnywhere, Category = "Jumping")
    FVector JumpImpulse = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Category = "Jumping")
    FVector JumpUpImpulse = FVector::ZeroVector;

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
